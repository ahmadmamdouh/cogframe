#ifndef CLICK_STATPACKETSENDER_HH
#define CLICK_STATPACKETSENDER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <elements/local/launch.hh>
CLICK_DECLS

/*
 * =c
 *
 * statPacketSender()
 *
 */

class StatPacketSender : public Element { public:

  StatPacketSender();
  ~StatPacketSender();

  const char *class_name() const		{ return "StatPacketSender"; }
  const char *port_count() const		{ return "0/1"; }
  const char *processing() const		{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  bool can_live_reconfigure() const		{ return true; }
  int initialize(ErrorHandler *);

//Function called by Statistics Collector to send request to all neighbors(broadcast)
  void sendMessage();


};

CLICK_ENDDECLS
#endif
