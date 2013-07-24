#ifndef CLICK_RFFrontEndUSRP_HH
#define CLICK_RFFrontEndUSRP_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elements/userlevel/todevice.hh>
#include <elements/local/RfFrontEnd.hh>
#include <elements/local/SpectrumManager.hh>

CLICK_DECLS

/*
 * =c
 *
 * spectrummanager()
 *
 */

class RFFrontEndUSRP : public Element, public RfFrontEnd { 
public:

  RFFrontEndUSRP();
  ~RFFrontEndUSRP();

  const char *class_name() const		{ return "RFFrontEndUSRP"; }
	const char *port_count() const		{ return PORTS_0_1; }
	const char *processing() const		{ return AGNOSTIC; }
	void change_channel(String, uint8_t,String);
	void scan(String);
    int configure(Vector<String> &, ErrorHandler *);
    
    private:
    SpectrumManager* _spectrum_manager;
};

CLICK_ENDDECLS
#endif
