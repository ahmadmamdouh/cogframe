#ifndef CLICK_EXPERIMENTENDCHECKER_HH
#define CLICK_EXPERIMENTENDCHECKER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <elements/userlevel/todevice.hh>
#include <elements/local/ExperimentEndHandler.hh>
#include <elements/local/Utilities.hh>
CLICK_DECLS

class ExperimentEndChecker : public Element { 
public:

  ExperimentEndChecker(){}
  ~ExperimentEndChecker(){}

  const char *class_name() const		{ return "ExperimentEndChecker"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

	private:
	long count;
	uint32_t terminateAt;
	String condition;
	ExperimentEndHandler* _eeh;
	long startTime;
};

CLICK_ENDDECLS
#endif
