#include <click/config.h>
#include <elements/local/SpectrumManager.hh>
#include <elements/local/Controller.cc>
#include <elements/local/Utilities.hh>
#include <elements/local/RfFrontEndFactory.cc>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <string>
#include "iwlib.h"
CLICK_DECLS

using namespace std;
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
	     _currentChannel = -1; // off/any 
	     	     
	return 0;
}

Packet *
SpectrumManager::simple_action(Packet *p_in)
{
   uint8_t channel = p_in->user_anno_u8(offset); //channel annotated in user_anno_u8
   if(channel != 0)//if channel = 0 then there is no channel annotated in the packet, so skip
   { 
	
	string ifName = Utilities::convert_String_to_string(if_name);
	int fromChannel = _currentChannel;
	int toChannel = channel;
	
	//printf("Elapsed time: %lld milliseconds\n", mtime);
	Controller::getInstance().setCurrentChannel(ifName,toChannel);
	rf->scan(if_name);
	long start = Utilities::getCurrentTime();
	rf->change_channel(if_name, channel,type);
	_currentChannel = channel;
	long end = Utilities::getCurrentTime();
	long switchTime = end - start;
	Controller::getInstance().addToSwitchTable(ifName,start,switchTime,fromChannel,toChannel);
	printf("average_switching_time %s %d\n",if_name.c_str(),switchTime);
   }
  return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SpectrumManager)

