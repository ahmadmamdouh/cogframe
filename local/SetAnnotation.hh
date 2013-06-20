#ifndef CLICK_SETANNOTATION_HH
#define CLICK_SETANNOTATION_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <elements/userlevel/todevice.hh>
#include <elements/local/Utilities.hh>
CLICK_DECLS

class SetAnnotation : public Element { 
public:

  SetAnnotation(){}
  ~SetAnnotation(){}

  const char *class_name() const		{ return "SetAnnotation"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

	private:
	uint32_t offset;
	uint32_t data;
};

CLICK_ENDDECLS
#endif
