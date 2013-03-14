#include <click/config.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrouter.hh>
#include <elements/local/launchlockrequester.hh>
#include <elements/local/LaunchLockResponseHandler.hh>
#include <elements/local/launchrtrsender.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <unistd.h>
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
		return; \
	} } while(0)







LaunchLockResponseHandler::LaunchLockResponseHandler()
{
	//_already_has_lock = 0;
}

LaunchLockResponseHandler::~LaunchLockResponseHandler()
{
}

int
LaunchLockResponseHandler::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
	  .read_mp("IF", _ifname)
	  .read_mp("IP", _ip)
	  .read_mp("LAUNCHRTRSENDER", reinterpret_cast<Element *&>(_rtr_sender))
      	  .read_mp("ROUTER", reinterpret_cast<Element *&>(_router))
	  .read_mp("TO_DEV", reinterpret_cast<Element *&>(_to_dev))
      .complete() < 0)
      return -1;

	return 0;
}




Packet *
LaunchLockResponseHandler::simple_action(Packet *p_in)
{
	click_chatter("RESPONDING TO LOCK");
	struct launch_ctrl_hdr * launch_hdr_ptr = (struct launch_ctrl_hdr *) (p_in->data()+14);
	if(launch_hdr_ptr->lock_response == 1)
	{
		click_chatter("RESPONDING TO LOCK1");
		
		if(_router->_already_has_lock == 0)
		{
			click_chatter("RESPONDING TO LOCK1.1");
			_router->_already_has_lock = 1;
			char buffer [3000];
			int n;

			printf("TAAANY############################\n");

			ErrorHandler  teet;
			_to_dev->initialize(&teet);
			p_in->set_user_anno_u8(0,launch_hdr_ptr->channel);
			_router->set_channel_loc_positive();
			return p_in;
		}
		else {
			click_chatter("RESPONDING TO LOCK1.2");
		}
	}
	else
	{
		click_chatter("RESPONDING TO LOCK2");
		_router->_already_has_lock = 0;
		_router->set_channel_loc_negative();
		_router->update_route(launch_hdr_ptr->neighbor_ip, launch_hdr_ptr->channel, launch_hdr_ptr->pu_behavior,launch_hdr_ptr->channel1, launch_hdr_ptr->pu_behavior1,launch_hdr_ptr->channel2, launch_hdr_ptr->pu_behavior2);
		return p_in;
	}
    return 0;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(LaunchLockResponseHandler)
