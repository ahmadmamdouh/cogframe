#include <click/config.h>
#include <elements/local/ProtocolPacketParser.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"


CLICK_DECLS

int
ProtocolPacketParser::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
				.read_mp("message", msgS)
	      .complete() < 0)
	      return -1;
	msg = Utilities::convert_String_to_string(msgS);
	return 0;
}

Packet *
ProtocolPacketParser::simple_action(Packet *p)
{

	long timestamp = Utilities::getCurrentTime();
	long fromTimestamp = -1;
	memcpy(&fromTimestamp, p->data() + 120, sizeof(fromTimestamp));
	click_ether *ethh = p->ether_header();
	uint8_t src_address[6];
	memcpy(src_address, ethh->ether_shost, 6);
	string from_mac = Utilities::convert_uint8_to_string(src_address);
	Controller::getInstance().addToProtocolTable(msg, fromTimestamp, timestamp,  from_mac,  Controller::getInstance().getAddress());
	return p;	

}

CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(ProtocolPacketParser)
