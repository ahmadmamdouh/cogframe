#ifndef LAUNCH_RTRHANDLER_HH
#define LAUNCH_RTRHANDLER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrouter.hh>
#include <click/confparse.hh>
CLICK_DECLS

class LaunchRTRHandler : public Element { public:

	LaunchRTRHandler();
	~LaunchRTRHandler();

	const char *class_name() const		{ return "LaunchRTRHandler"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

	int configure(Vector<String> &, ErrorHandler *);


	Packet *simple_action(Packet *);

	private:



	//Pointer to LaunchRouter to insert response
	LaunchRouter * _router;


};
CLICK_ENDDECLS
#endif
