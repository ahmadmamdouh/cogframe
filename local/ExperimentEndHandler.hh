#ifndef EXPERIMENTENDHANDLER_HH
#define EXPERIMENTENDHANDLER_HH
#include <click/element.hh>
#include <click/confparse.hh>
#include <elements/standard/timedsource.hh>
#include <elements/local/StatPacketSender.hh>
#include <elements/local/CogHeader.hh>
#include <elements/local/Utilities.hh>
#include <elements/local/Controller.cc>
#include <click/handlercall.hh>


CLICK_DECLS


class ExperimentEndHandler : public Element { public:

    ExperimentEndHandler(){}
    ~ExperimentEndHandler(){}

    const char *class_name() const	{ return "ExperimentEndHandler"; }
    const char *port_count() const	{ return PORTS_0_1; }
    const char *processing() const	{ return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);
	void endExperiment();
	
	private:
	int count,maxNumPackets;
	TimedSource* ts;
	String nodeType;
	CogHeader _lh;
};

CLICK_ENDDECLS
#endif
