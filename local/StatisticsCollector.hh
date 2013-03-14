#ifndef CLICK_STATISTICSCOLLECTOR_HH
#define CLICK_STATISTICSCOLLECTOR_HH
#include <click/element.hh>
#include <click/confparse.hh>
#include <elements/standard/timedsource.hh>
#include <elements/local/StatPacketSender.hh>
CLICK_DECLS





class StatisticsCollector : public Element { public:

    StatisticsCollector();
    ~StatisticsCollector();

    const char *class_name() const	{ return "StatisticsCollector"; }
    const char *port_count() const	{ return PORTS_1_1; }
    
    int configure(Vector<String> &, ErrorHandler *);
    int initialize(ErrorHandler *);

    Packet *simple_action(Packet *);

  private:
    TimedSource* _ts;
    uint64_t _count;
    StatPacketSender* _sender;

   
};

CLICK_ENDDECLS
#endif
