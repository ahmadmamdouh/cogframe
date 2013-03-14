#ifndef CLICK_STATPACKETPARSER_HH
#define CLICK_STATPACKETPARSER_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
 * =c
 *
 * statPacketParser()
 *
 */

class StatPacketParser : public Element { public:

  StatPacketParser();
  ~StatPacketParser();

  const char *class_name() const		{ return "StatPacketParser"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  bool can_live_reconfigure() const		{ return true; }
  int initialize(ErrorHandler *);
  Packet* simple_action(Packet *p_in);
};

CLICK_ENDDECLS
#endif
