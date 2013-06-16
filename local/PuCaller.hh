#ifndef CLICK_PUCALLER_HH
#define CLICK_PUCALLER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <set>
#include <elements/userlevel/todevice.hh>
#include <elements/local/launchrcvrlock.hh>

CLICK_DECLS

/*
 * =c
 *
 * pucaller()
 *
 */

class PuCaller : public Element { 
public:

  PuCaller();
  ~PuCaller();

  const char *class_name() const		{ return "PuCaller"; }
  const char *port_count() const		{ return PORTS_1_1; }
  const char *processing() const		{ return AGNOSTIC; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

private:
  LaunchLockResponder* _llr;
  int _count;

};

CLICK_ENDDECLS
#endif
