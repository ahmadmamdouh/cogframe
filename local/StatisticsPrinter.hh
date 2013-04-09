#ifndef STATISTICSPRINTER_HH
#define STATISTICSPRINTER_HH
#include <click/element.hh>
#include <click/confparse.hh>
#include <elements/local/Utilities.hh>
#include <elements/local/Controller.cc>
#include <click/handlercall.hh>


CLICK_DECLS


class StatisticsPrinter : public Element { public:

    StatisticsPrinter(){}
    ~StatisticsPrinter(){}

    const char *class_name() const	{ return "StatisticsPrinter"; }
    const char *port_count() const	{ return PORTS_1_1; }
    const char *processing() const	{ return AGNOSTIC; }

    int configure(Vector<String> &, ErrorHandler *);
	Packet *simple_action(Packet *);
	
};

CLICK_ENDDECLS
#endif
