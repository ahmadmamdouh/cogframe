#ifndef ProtocolPacketParser_HH
#define ProtocolPacketParser_HH
#include <click/element.hh>
#include <click/confparse.hh>
#include <elements/local/Utilities.hh>
#include <elements/local/Controller.cc>
#include <click/handlercall.hh>


CLICK_DECLS


class ProtocolPacketParser : public Element { public:

    ProtocolPacketParser(){}
    ~ProtocolPacketParser(){}

    const char *class_name() const	{ return "ProtocolPacketParser"; }
    const char *port_count() const	{ return PORTS_1_1; }
    const char *processing() const	{ return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);
		Packet *simple_action(Packet *);

  private:
  string msg;
  String msgS;		
};

CLICK_ENDDECLS
#endif
