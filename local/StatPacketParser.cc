#include <click/config.h>
#include <elements/local/StatPacketParser.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>

CLICK_DECLS

StatPacketParser::StatPacketParser()
{
}

StatPacketParser::~StatPacketParser()
{
}

int
StatPacketParser::configure(Vector<String> &conf, ErrorHandler *errh)
{
  
  return 0;
}

int
StatPacketParser::initialize(ErrorHandler *)
{
  return 0;
}


Packet *
StatPacketParser::simple_action(Packet *p_in)
{
   uint8_t a[100];
   memcpy(a,(p_in->data()+15),sizeof(a));
   for(int i = 0; i<100; i++)
     printf("%d\n", a[i]);
   return p_in;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(StatPacketParser)
