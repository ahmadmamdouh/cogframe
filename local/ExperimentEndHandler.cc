#include <click/config.h>
#include <elements/local/ExperimentEndHandler.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"


CLICK_DECLS

int
ExperimentEndHandler::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_mp("TimedSource", reinterpret_cast<Element *&>(ts)) // interface name that will switch the channel.
	    .complete() < 0)
	      return -1;
	count = 0;
		      
	return 0;
}


void
ExperimentEndHandler::endExperiment(){
	// terminate the exp
	HandlerCall::call_write(ts,"active","false");
	// call controller to print statistics file
	Controller::getInstance().printStatsFile();
	// broadcast to all nodes informing them that the exp has ended
	int annotation = 9; // Interpreted as hexadecimal number // end of exp annotation
	WritablePacket *packet = Packet::make((void *)&_lh, sizeof(_lh));
	if (packet == 0 )
		click_chatter( "cannot make packet!");
	struct CogHeader *format = (struct CogHeader*) packet->data();
	format->type = 1; //exp ended
	uint8_t source_eth[6],dest_eth[6];
	Utilities::convert_String_to_uint8_t(Controller::getInstance().getAddress(),source_eth);

	for(int i = 0; i < 6; i++)
		dest_eth[i] = 255;	
	
	char bufferr[100];
	sprintf(bufferr, "%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x\0", source_eth[0], source_eth[1],
				source_eth[2], source_eth[3], source_eth[4], source_eth[5]);
	printf("discover packet making %s\n",bufferr);
	

	
	//set the source and dist adresses into ether header
	struct click_ether ethh;
	memcpy(ethh.ether_shost, source_eth,6);
	memcpy(ethh.ether_dhost, dest_eth, 6);
	//set ether type so that we can identify the packet at filter
	ethh.ether_type = 0x1234;
	//Push Ethernet header
	WritablePacket *q = packet->push(14);
	memcpy(q->data(), &ethh, 14);
		
	// output the discovery packet to output 0
	output(0).push(q);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ExperimentEndHandler)
