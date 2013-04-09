#include <click/config.h>
#include <elements/local/ExperimentEndChecker.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"


CLICK_DECLS

int
ExperimentEndChecker::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_p("ExperimentEndHandler", reinterpret_cast<Element *&>(_eeh)) 
		.read_mp("maxNumPackets",maxNumPackets)
	    .complete() < 0)
	      return -1;
	count = 0;
		      
	return 0;
}



Packet *
ExperimentEndChecker::simple_action(Packet *p)
{
	printf("SRC_COUNT: %d",count);
	if(count == maxNumPackets){
		_eeh->endExperiment();
		 return 0;
	}
	memcpy(p->data()+50,&count,sizeof(count));
	count++;	
	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ExperimentEndChecker)
