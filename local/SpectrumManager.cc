#include <click/config.h>
#include <elements/local/SpectrumManager.hh>
#include <elements/local/RfFrontEndFactory.cc>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"
CLICK_DECLS


/**
This module is responsible for Spectrum management using WI-FI card
Channel number is annotated in the incoming packet in the user_anno_u8 header, and the offset is given as a parameter.
*/

SpectrumManager::SpectrumManager()
{
}

SpectrumManager::~SpectrumManager()
{
}

int
SpectrumManager::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_mp("if_name", if_name) // interface name that will switch the channel.
		.read_mp("offset", offset)   // offset in user_anno_u8 where channel number is located.
		.read_mp("type", type)   // type: T: transmitter or R: reciever
		.read_mp("rftype", rfType)   // type: T: transmitter or R: reciever
	      .complete() < 0)
	      return -1;
	      
	     RfFrontEndFactory rfFactory;
	     rf = rfFactory.getRfFrontEnd(rfType);
	return 0;
}




Packet *
SpectrumManager::simple_action(Packet *p_in)
{
   uint8_t channel = p_in->user_anno_u8(offset); //channel annotated in user_anno_u8
   if(channel != 0)//if channel = 0 then there is no channel annotated in the packet, so skip
   { 
	rf->scan(if_name);
	rf->change_channel(if_name, channel,type);
   }
  return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SpectrumManager)

