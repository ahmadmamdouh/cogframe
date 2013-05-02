#ifndef CLICK_DiscoverNeighbors_HH
#define CLICK_DiscoverNeighbors_HH
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <click/string.hh>
#include <click/glue.hh>
#include <elements/local/CogHeader.hh>
CLICK_DECLS


class DiscoverNeighbors: public Element {
private:
	CogHeader _lh;
  void operator=(DiscoverNeighbors const&); // Don't implement	
	
public:
	//Controller controller = Controller::getInstance();
	DiscoverNeighbors();
	~DiscoverNeighbors();
	
	const char *class_name() const		{ return "DiscoverNeighbors"; }
	const char *port_count() const		{ return "0/1"; }
	const char *processing() const		{ return PUSH; }

	int configure(Vector<String> &, ErrorHandler *);
	bool can_live_reconfigure() const		{ return true; }
	int initialize(ErrorHandler *);

	void discover();
	static DiscoverNeighbors& getInstance();
};

CLICK_ENDDECLS
#endif /* DiscoverNeighbors_HH_ */
