#ifndef CLICK_SPECTRUMMANAGER_HH
#define CLICK_SPECTRUMMANAGER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elements/userlevel/todevice.hh>
#include <elements/local/RfFrontEnd.hh>

CLICK_DECLS

/*
 * =c
 *
 * spectrummanager()
 *
 */

class SpectrumManager : public Element { 
public:

  SpectrumManager();
  ~SpectrumManager();

  const char *class_name() const		{ return "SpectrumManager"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }

  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);
 // void change_channel(String ,uint8_t );

	private:
	String if_name;
	uint8_t offset;
	String type;
	String rfType;
	int _currentChannel;
	RfFrontEnd* rf;


};

CLICK_ENDDECLS
#endif
