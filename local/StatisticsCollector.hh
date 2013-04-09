#ifndef CLICK_STATISTICSCOLLECTOR_HH
#define CLICK_STATISTICSCOLLECTOR_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <elements/userlevel/todevice.hh>
#include <elements/local/Utilities.hh>
#include <elements/local/ExperimentEndHandler.hh>
CLICK_DECLS

/*
 * =c
 *
 * spectrummanager()
 *
 */

class StatisticsCollector : public Element { 
public:

  StatisticsCollector();
  ~StatisticsCollector();

  const char *class_name() const		{ return "StatisticsCollector"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

	private:
	String isSrc,type;
	int count;
	ExperimentEndHandler *_eeh;
};

CLICK_ENDDECLS
#endif
