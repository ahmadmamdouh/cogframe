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
		.read_mp("CH0", _pu_behavior0)
		.read_mp("CH1", _pu_behavior1)
		.read_mp("CH2", _pu_behavior2)
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
	

	struct launch_ctrl_hdr * launch_hdr = (struct launch_ctrl_hdr *) (p_in->data()+14);

	if(_lock_count ==0)
	{

		char buffer [3000];
		int n;

		p->set_user_anno_u8(0,lauch_hdr_ptr->channel);

		click_chatter("Make Lock Rsponse Packet");
		ErrorHandler errh;
		_from_dev->initialize(&errh);
		_locked_channel = lauch_hdr_ptr->channel;
		lauch_hdr_ptr->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
		lauch_hdr_ptr->lock_response = 1;
		_lock_count++;
		click_ether *ethh = p->ether_header();
		uint8_t source_address[6];
		memcpy(source_address, ethh->ether_shost, 6);
		memcpy(ethh->ether_shost, ethh->ether_dhost, 6);
		memcpy(ethh->ether_dhost, source_address, 6);


		_lock_timeout_timer.schedule_after_msec(_lock_timeout_ms);


		return p;
	}
	if((_lock_count > 0 && lauch_hdr_ptr->channel == _locked_channel))
	{

		_locked_channel = lauch_hdr_ptr->channel;
		lauch_hdr_ptr->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
		lauch_hdr_ptr->lock_response = 1;
		_lock_count++;
		click_ether *ethh = p->ether_header();
		uint8_t source_address[6];
		memcpy(source_address, ethh->ether_shost, 6);
		memcpy(ethh->ether_shost, ethh->ether_dhost, 6);
		memcpy(ethh->ether_dhost, source_address, 6);

		_lock_timeout_timer.schedule_after_msec(_lock_timeout_ms);
		return p;
	}else{
		char buffer [3000];
		int n;

		p->set_user_anno_u8(0,lauch_hdr_ptr->channel);

		click_chatter("Make Lock Rsponse Packet 2");
		ErrorHandler errh;
		_from_dev->initialize(&errh);
		_locked_channel = lauch_hdr_ptr->channel;
		lauch_hdr_ptr->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
		lauch_hdr_ptr->lock_response = 1;
		_lock_count++;
		click_ether *ethh = p->ether_header();
		uint8_t source_address[6];
		memcpy(source_address, ethh->ether_shost, 6);
		memcpy(ethh->ether_shost, ethh->ether_dhost, 6);
		memcpy(ethh->ether_dhost, source_address, 6);


		_lock_timeout_timer.schedule_after_msec(_lock_timeout_ms);


		return p;
	}
	lauch_hdr_ptr->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
	lauch_hdr_ptr->lock_response = 0;

	switch(_locked_channel)
	{
		case 1:
			lauch_hdr_ptr->channel = 1;
			lauch_hdr_ptr->pu_behavior = _pu_behavior0;
			break;
		case 6:
			lauch_hdr_ptr->channel = 6;
			lauch_hdr_ptr->pu_behavior = _pu_behavior1;
			break;
		case 11:
			lauch_hdr_ptr->channel = 11;
			lauch_hdr_ptr->pu_behavior = _pu_behavior2;
			break; 
	}


	click_ether *ethh = p->ether_header();
	uint8_t source_address[6];
	memcpy(source_address, ethh->ether_shost, 6);
	memcpy(ethh->ether_shost, ethh->ether_dhost, 6);
	memcpy(ethh->ether_dhost, source_address, 6);


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

	//this machine can no longer use the sensed channel
	llr->_router->cant_use_channel(channel_sensed);
	llr->_router->set_channel_loc_negative();
	llr->_router->set_ready_for_another_packet_negative();
	


	if(llr->_locked_channel == channel_sensed)
	{	
		llr->_locked_channel = 0;
		llr->_lock_count = 0;
	}

	//neighbors shouldn't communicate with this machine on the sensed channel
	//send LOCK_RES to update the state of the lock
	struct launch_ctrl_hdr  _lh;




	if(!llr->_router->can_use_1)
	{
		if(llr->_pu_behavior1 <= llr->_pu_behavior2 &&llr->_router->can_use_6)
		{
			click_chatter("CONNECTING TO 6");
			_lh.channel=6;
			_lh.pu_behavior = llr->_pu_behavior1;
		}
		else if(llr->_pu_behavior2 <= llr->_pu_behavior1 &&llr->_router->can_use_11)
		{
			click_chatter("CONNECTING TO 11");
			_lh.channel=11;
			_lh.pu_behavior = llr->_pu_behavior2;
		}
		else
		{
			click_chatter("1 ... ANA MSH 3AREFNY .. ANA TOT MEHY ... ANA MSH HOP !!");
			click_chatter("7RO7 2DWAR .. 7RO7 2DWAR .. 7RO7 2DWAR ... 3ALA SPECTRUM !!");
			click_chatter("CHRASHING .... .... !!!!!!");
			//exit(0);			
		}
	}
	else if(!llr->_router->can_use_6)
	{
		if(llr->_pu_behavior0 <= llr->_pu_behavior2 &&llr->_router->can_use_1)
		{
			click_chatter("CONNECTING TO 1");
			_lh.channel=1;
			_lh.pu_behavior = llr->_pu_behavior0;
		}
		else if(llr->_pu_behavior2 <= llr->_pu_behavior0 &&llr->_router->can_use_11)
		{
			click_chatter("CONNECTING TO 11");
			_lh.channel=11;
			_lh.pu_behavior = llr->_pu_behavior2;
		}
		else
		{
			click_chatter("6 ... ANA MSH 3AREFNY .. ANA TOT MEHY ... ANA MSH HOP !!");
			click_chatter("7RO7 2DWAR .. 7RO7 2DWAR .. 7RO7 2DWAR ... 3ALA SPECTRUM !!");
			click_chatter("CHRASHING .... .... !!!!!!");
			//exit(0);			
		}

	}
	else if(!llr->_router->can_use_11)
	{
		if(llr->_pu_behavior1 <= llr->_pu_behavior0 &&llr->_router->can_use_6)
		{
			click_chatter("CONNECTING TO 6");
			_lh.channel=6;
			_lh.pu_behavior = llr->_pu_behavior1;
		}
		else if(llr->_pu_behavior0 <= llr->_pu_behavior1 &&llr->_router->can_use_1)
		{
			click_chatter("CONNECTING TO 1");
			_lh.channel=1;
			_lh.pu_behavior = llr->_pu_behavior0;
		}
		else
		{
			click_chatter("11 ... ANA MSH 3AREFNY .. ANA TOT MEHY ... ANA MSH HOP !!");
			click_chatter("7RO7 2DWAR .. 7RO7 2DWAR .. 7RO7 2DWAR ... 3ALA SPECTRUM !!");
			click_chatter("CHRASHING .... .... !!!!!!");
			//exit(0);			
		}

	}
	else
	{
		click_chatter("mfesh 7aga naf3a");
		click_chatter("ANA MSH 3AREFNY .. ANA TOT MEHY ... ANA MSH HOP !!");
		click_chatter("7RO7 2DWAR .. 7RO7 2DWAR .. 7RO7 2DWAR ... 3ALA SPECTRUM !!");
		click_chatter("CHRASHING .... .... !!!!!!");
		//exit(0);
	}


	WritablePacket *packet = Packet::make((void *)&_lh, sizeof(_lh));

	if (packet == 0 )
	{	
		click_chatter( "cannot make packet!");
		return 0;
	} 
	struct launch_ctrl_hdr * format = (struct launch_ctrl_hdr *) packet->data();

	format->type = launch_ctrl_hdr::LAUNCH_LOCK_RES;
	format->lock_response = 0;
	format->neighbor_ip = llr->_ip;
	format->channel =1;
	format->channel1 =6;
	format->channel2 =11;

	format->pu_behavior = (uint32_t)(llr->_pu_behavior0);
	format->pu_behavior1 = (uint32_t)(llr->_pu_behavior1);
	format->pu_behavior2 = (uint32_t)(llr->_pu_behavior2);

	if(!llr->_router->can_use_1)
	{
		llr->_pu_behavior0 = 2000;
		format->pu_behavior = 2000;
	}
	if(!llr->_router->can_use_6)
	{
		llr->_pu_behavior1 = 2000;
		format->pu_behavior1 = 2000;
	}
	if(!llr->_router->can_use_11)
	{
		llr->_pu_behavior2 = 2000;
		format->pu_behavior2 = 2000;
	}

	printf("SENDING PU BEHAVIORS %d %d %d \n", format->pu_behavior, format->pu_behavior1, format->pu_behavior2);
	//format->lock_response = 0;	
	//Push Ethernet header
	struct click_ether ethh;


	memset(ethh.ether_dhost,0xff,6);

	memcpy(ethh.ether_shost, llr->_my_eth.data(), 6);
	ethh.ether_type = 0x0007;
 
	WritablePacket *q = packet->push(14);
	memcpy(q->data(), &ethh, 14);
	q->set_user_anno_u8(0,_lh.channel);
	llr->output(0).push(q);

    return 0;
}


void
LaunchLockResponder::add_handlers() {
    add_write_handler("pu_sensed", pu_sensed, 0);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(LaunchLockResponder)
