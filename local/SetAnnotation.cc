#include <click/config.h>
#include <elements/local/SetAnnotation.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"
#include <elements/local/Utilities.hh>

CLICK_DECLS

int
SetAnnotation::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_p("offset", offset) 
		.read_mp("Data",data)
	    .complete() < 0)
	      return -1;
	return 0;
}



Packet *
SetAnnotation::simple_action(Packet *p)
{
	memcpy(p->data()+offset,&data,sizeof(data));
	return p;
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(SetAnnotation)
