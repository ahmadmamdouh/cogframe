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
	WritablePacket *packet = Packet::make((void *)&annotation, sizeof(annotation));
	if (packet == 0 )
		click_chatter( "cannot make packet!");
	output(0).push(packet);
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ExperimentEndHandler)
