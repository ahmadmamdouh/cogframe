#ifndef CLICK_CogHeaderParser_HH
#define CLICK_CogHeaderParser_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <elements/local/CogHeader.hh>
#include <elements/local/Controller.cc>

CLICK_DECLS

/*
 * =c
 *
 * CogHeaderParser()
 *
 */

class CogHeaderParser : public Element { public:

  CogHeaderParser();
  ~CogHeaderParser();

  const char *class_name() const		{ return "CogHeaderParser"; }
  const char *port_count() const		{ return "1/-"; }
  const char *processing() const		{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet* simple_action(Packet *p_in);
};

CLICK_ENDDECLS
#endif
