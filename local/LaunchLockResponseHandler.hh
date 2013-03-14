#ifndef LAUNCH_LOCK_RESPONSE_HANDLER_HH
#define LAUNCH_LOCK_RESPONSE_HANDLER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrouter.hh>
#include <elements/local/launchlockrequester.hh>
#include <click/confparse.hh>
#include <elements/userlevel/todevice.hh>
#include <elements/local/launchrtrsender.hh>
#include<stdio.h>
#include<stdlib.h>
#include "iwlib.h"

#include <string.h>


#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>


CLICK_DECLS

class LaunchLockResponseHandler : public Element { public:

	LaunchLockResponseHandler();
	~LaunchLockResponseHandler();

	const char *class_name() const		{ return "LaunchLockResponseHandler"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

	int configure(Vector<String> &, ErrorHandler *);

	Packet *simple_action(Packet *);

//	void change_channel(String ,uint8_t );

	private:

	String _ifname;
	IPAddress _ip;

	//Pointer to LaunchRouter to insert response
	LaunchRouter * _router;
	//Pointer to intitialize ToDevice Element
	ToDevice * _to_dev;

	LaunchRTRSender * _rtr_sender;
	//int _already_has_lock;
};
CLICK_ENDDECLS
#endif
