#include <click/config.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrcvrlock.hh>
#include <elements/local/launchrtrsender.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/straccum.hh>
#include<stdio.h>
#include<stdlib.h>
#include "iwlib.h"

#include <string.h>


#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>

CLICK_DECLS

#define IW_SET_EXT_ERR(skfd, ifname, request, wrq, rname) \
	do { \
	if(iw_set_ext(skfd, ifname, request, wrq) < 0) { \
		fprintf(stderr, "    SET failed on device %-1.16s ; %s.\n", \
			ifname, strerror(errno)); \
		return(-5); \
	} } while(0)


LaunchLockResponder::LaunchLockResponder()
: _lock_timeout_timer(this)
{
	_locked_channel = 0;
	_lock_count = 0;
}

LaunchLockResponder::~LaunchLockResponder()
{
}

int
LaunchLockResponder::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_mp("IP", _ip)
		.read_mp("DEVNAME", _ifname)
		.read_mp("WAIT", _lock_timeout_ms)
		.read_p("FROM_DEV", reinterpret_cast<Element *&>(_from_dev))
		.read_mp("ETH", _my_eth)
		.read_p("ROUTER", reinterpret_cast<Element *&>(_router))
	  	.read_mp("LAUNCHRTRSENDER", reinterpret_cast<Element *&>(_rtr_sender))
		.complete() < 0)
      return -1;

	_lock_timeout_timer.initialize(this);

	
	return 0;
}



Packet *
LaunchLockResponder::simple_action(Packet *p_in)
{
	WritablePacket *p = p_in->uniqueify();

	struct launch_ctrl_hdr * lauch_hdr_ptr = (launch_ctrl_hdr *) (p->data()+14);
	lauch_hdr_ptr->neighbor_ip = _ip;

	click_ether *ethh = p->ether_header();
	// Swap Source and Dest addresses
	uint8_t source_address[6];
	memcpy(source_address, ethh->ether_shost, 6);
	memcpy(ethh->ether_shost, ethh->ether_dhost, 6);
	memcpy(ethh->ether_dhost, source_address, 6);	

	lauch_hdr_ptr->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
	lauch_hdr_ptr->lock_response = 1;
	_lock_timeout_timer.schedule_after_msec(_lock_timeout_ms);
	
	if(_lock_count == 0) {
		p->set_user_anno_u8(0,lauch_hdr_ptr->channel_used);
		click_chatter("Make Lock Response Packet");
		ErrorHandler errh;
		_from_dev->initialize(&errh);
		_locked_channel = lauch_hdr_ptr->channel_used;
	}
	
	if(lauch_hdr_ptr->channel_used != _locked_channel) {
		p->set_user_anno_u8(0,lauch_hdr_ptr->channel_used);
		click_chatter("Make Lock Response Packet 2");
		ErrorHandler errh;
		_from_dev->initialize(&errh);
		_locked_channel = lauch_hdr_ptr->channel_used;
		lauch_hdr_ptr->lock_response = 0;
	}
	
	_lock_count++;
	return p;
}


void
LaunchLockResponder::run_timer(Timer * t)
{
	
}


int
LaunchLockResponder::pu_sensed(const String &s, Element *e, void *, ErrorHandler *errh)
{
  LaunchLockResponder *llr = static_cast<LaunchLockResponder *>(e);
	uint32_t channel_sensed = atoi(s.c_str());
	printf("^_^_^_^_^_^_^ %d\n", channel_sensed);
	//this machine can no longer use the sensed channel
	llr->_router->cant_use_channel(channel_sensed);
	llr->_router->set_channel_loc_negative();
	llr->_router->set_ready_for_another_packet_negative();
	

	if(llr->_locked_channel == channel_sensed) {	
		llr->_locked_channel = 0;
		llr->_lock_count = 0;
	}

	//neighbors shouldn't communicate with this machine on the sensed channel
	//send LOCK_RES to update the state of the lock
	struct launch_ctrl_hdr  _lh;
	Controller controller = Controller::getInstance();
	vector<int> *channel_ids = controller.get_channels();
	_lh.channels_size = (*channel_ids).size();
	int new_channel_id = -1;
	float min_pu_prob = 1<<28;
	for(int i=0;i<(*channel_ids).size();i++) {
		int channel_id = (*channel_ids)[i];
		float pu_prob = controller.get_pu_prob(channel_id);
		if(!controller.is_pu_active(channel_id)){
			if(pu_prob < min_pu_prob) {
				min_pu_prob = pu_prob;
				new_channel_id = channel_id;
			}
		}
	}
	
	if(new_channel_id == -1){
		printf("Can't find new channel\n");
	}
	_lh.channel_used = new_channel_id;
	
	WritablePacket *packet = Packet::make((void *)&_lh, sizeof(_lh));

	if (packet == 0 ) {	
		click_chatter( "cannot make packet!");
		return 0;
	} 
	struct launch_ctrl_hdr * format = (struct launch_ctrl_hdr *) packet->data();

	format->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
	format->lock_response = 0;
	format->neighbor_ip = llr->_ip;
	printf("------------> Channels Size %d\n",format->channels_size);
	for(int i = 0; i < format->channels_size; i++) {
		uint8_t channel_id = (*channel_ids)[i];
		printf("################    %d\n",channel_id);
		format->channels_id[i] = channel_id;
		if(controller.is_pu_active(channel_id))
			controller.set_pu_prob(channel_id, 1);
		format->channels_pu_prob[i] = controller.get_pu_prob(channel_id);
	}

	//printf("SENDING PU BEHAVIORS %d %d %d \n", format->pu_behavior, format->pu_behavior1, format->pu_behavior2);
	//format->lock_response = 0;	
	//Push Ethernet header
	struct click_ether ethh;

	memset(ethh.ether_dhost,0xff,6);

	memcpy(ethh.ether_shost, llr->_my_eth.data(), 6);
	ethh.ether_type = 0x0007;
 
	WritablePacket *q = packet->push(14);
	memcpy(q->data(), &ethh, 14);
	q->set_user_anno_u8(0,new_channel_id);
	llr->output(0).push(q);

  return 0;
}


void
LaunchLockResponder::add_handlers() {
    add_write_handler("pu_sensed", pu_sensed, 0);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(LaunchLockResponder)
