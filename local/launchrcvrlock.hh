#ifndef LAUNCHLOCKRESPONDER_HH
#define LAUNCHLOCKRESPONDER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <elements/local/launch.hh>
#include <click/confparse.hh>
#include <click/timer.hh>
#include <elements/local/launchrouter.hh>
#include <elements/local/launchrtrsender.hh>
#include <elements/userlevel/fromdevice.hh>
#include<stdio.h>
#include<stdlib.h>
#include "iwlib.h"

#include <string.h>


#include <netinet/ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>

CLICK_DECLS

class LaunchLockResponder : public Element { public:

	LaunchLockResponder();
	~LaunchLockResponder();

	const char *class_name() const		{ return "LaunchLockResponder"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }
	void add_handlers();
	int configure(Vector<String> &, ErrorHandler *);
	void run_timer(Timer *);
//	void LaunchLockResponder::change_channel(String _if_name,uint8_t channel);

	Packet *simple_action(Packet *);
	LaunchRouter * _router;
	EtherAddress _my_eth;


//This node's ip address	
	IPAddress _ip;

	private:
	String _ifname;
//Channel locked 	
	uint8_t _locked_channel;
//Lock counter	
	uint8_t _lock_count;
//channel will be locked for time = _loc_timeout_ms in millisecond	
	uint32_t _lock_timeout_ms;
//Timer of the lock	
	Timer _lock_timeout_timer;

	FromDevice * _from_dev;

	LaunchRTRSender * _rtr_sender;


// Handler for Primary User sensor
	static int pu_sensed(const String &s, Element *e, void *, ErrorHandler *errh);
	static int pu_unsensed(const String &s, Element *e, void *, ErrorHandler *errh);

//The header of Launch control packet
	struct launch_ctrl_hdr _lh;


};
CLICK_ENDDECLS
#endif
