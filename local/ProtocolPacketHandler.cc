#include <click/config.h>
#include <elements/local/ProtocolPacketHandler.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"


CLICK_DECLS

int
ProtocolPacketHandler::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
	    .complete() < 0)
	      return -1;
	return 0;
}

Packet *
ProtocolPacketHandler::simple_action(Packet *p)
{
	struct timeval start;
	long mtime, seconds, useconds;    
	gettimeofday(&start, NULL);
	seconds  = start.tv_sec; // seconds since epoch
	useconds = start.tv_usec; // microSeconds since epoch
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	
	memcpy(p->data() + 120, &mtime, sizeof(mtime));
	return p;	
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ProtocolPacketHandler)
