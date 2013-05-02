#ifndef ProtocolPacketHandler_HH
#define ProtocolPacketHandler_HH
#include <click/element.hh>
#include <click/confparse.hh>
#include <elements/local/Utilities.hh>
#include <elements/local/Controller.cc>
#include <click/handlercall.hh>


CLICK_DECLS


class ProtocolPacketHandler : public Element { public:

    ProtocolPacketHandler(){}
    ~ProtocolPacketHandler(){}

    const char *class_name() const	{ return "ProtocolPacketHandler"; }
    const char *port_count() const	{ return PORTS_1_1; }
    const char *processing() const	{ return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);
		Packet *simple_action(Packet *);
	
};

CLICK_ENDDECLS
#endif
