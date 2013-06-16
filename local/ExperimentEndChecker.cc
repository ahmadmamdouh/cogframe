#include <click/config.h>
#include <elements/local/ExperimentEndChecker.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"
#include <elements/local/Utilities.hh>

CLICK_DECLS

int
ExperimentEndChecker::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_p("ExperimentEndHandler", reinterpret_cast<Element *&>(_eeh)) 
		.read_mp("condition",condition)
		.read_mp("terminateAt",terminateAt)
	    .complete() < 0)
	      return -1;
	count = 0;
	startTime = Utilities::getCurrentTime();
	return 0;
}



Packet *
ExperimentEndChecker::simple_action(Packet *p)
{

	if(condition == "numPackets")
		count++;	
	else
		count = Utilities::getCurrentTime() - startTime;
	printf("SRC_COUNT: %d",count);
	
	if(count > terminateAt){
		_eeh->endExperiment();
		 return 0;
	}
	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ExperimentEndChecker)
