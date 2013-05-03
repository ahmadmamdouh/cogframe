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

	long timestamp = Utilities::getCurrentTime();
	memcpy(p->data() + 120, &timestamp, sizeof(timestamp));
	return p;	
}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ProtocolPacketHandler)
