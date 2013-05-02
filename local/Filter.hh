#ifndef CLICK_FILTER_HH
#define CLICK_FILTER_HH
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <elements/local/Controller.cc>
#include <elements/local/DiscoverNeighbors.hh>
#include <click/string.hh>
#include <click/glue.hh>
#include <elements/local/launch.hh>
CLICK_DECLS


class Filter: public Element {
public:
	//Controller controller = Controller::getInstance();

	Filter();
	~Filter();
	bool disc;
	const char *class_name() const {
		return "Filter";
	}
	const char *port_count() const {
		return "1/-";
	}
	
	
	int configure(Vector<String> &, ErrorHandler *);
	bool can_live_reconfigure() const		{ return true; }
	int initialize(ErrorHandler *);
	
	Packet *simple_action(Packet *);

};

CLICK_ENDDECLS
#endif /* FILTER_HH_ */
