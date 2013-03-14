#include <click/config.h>
#include <elements/local/StatPacketSender.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>

CLICK_DECLS

StatPacketSender::StatPacketSender()
{
}

StatPacketSender::~StatPacketSender()
{
}

int
StatPacketSender::configure(Vector<String> &conf, ErrorHandler *errh)
{
  
  return 0;
}

int
StatPacketSender::initialize(ErrorHandler *)
{
  return 0;
}


void
StatPacketSender::sendMessage()
{
	int annotation = 9; // Interpreted as hexadecimal number
	WritablePacket *packet = Packet::make((void *)&annotation, sizeof(annotation));

	if (packet == 0 )
		return click_chatter( "cannot make packet!");

	 output(0).push(packet);
	
}


CLICK_ENDDECLS
EXPORT_ELEMENT(StatPacketSender)
