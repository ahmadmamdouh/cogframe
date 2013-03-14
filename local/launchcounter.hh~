#ifndef CLICK_LAUNCHCOUNTER_HH
#define CLICK_LAUNCHCOUNTER_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS





class LaunchCounter : public Element { public:

    LaunchCounter();
    ~LaunchCounter();

    const char *class_name() const	{ return "LaunchCounter"; }
    const char *port_count() const	{ return PORTS_1_1; }

    int initialize(ErrorHandler *);

    Packet *simple_action(Packet *);

  private:

    double _usec_accum;
    uint64_t _count;

   
};

CLICK_ENDDECLS
#endif
