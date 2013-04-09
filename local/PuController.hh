#ifndef CLICK_PU_CONTROLLER_HH
#define CLICK_PU_CONTROLLER_HH
#include <click/element.hh>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <elements/userlevel/todevice.hh>

using namespace std;

CLICK_DECLS


class PuController : public Element { 
public:

  PuController();
  ~PuController();

  const char *class_name() const		{ return "PuController"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);
	
	private:
	int can_send;
	int time_to_live;
	String _active, _inactive;

};

CLICK_ENDDECLS
#endif
