#include <click/config.h>
#include <elements/local/StatisticsPrinter.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"


CLICK_DECLS

int
StatisticsPrinter::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
	    .complete() < 0)
	      return -1;
		      
	return 0;
}

Packet *
StatisticsPrinter::simple_action(Packet *p)
{
	Controller::getInstance().printStatsFile();
	return 0;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(StatisticsPrinter)
