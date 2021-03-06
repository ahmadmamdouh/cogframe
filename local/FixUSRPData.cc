#include <click/config.h>
#include <elements/local/launch.hh>
#include <elements/local/FixUSRPData.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
CLICK_DECLS

FixUSRPData::FixUSRPData()
{
}

FixUSRPData::~FixUSRPData()
{
}

int
FixUSRPData::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
      .complete() < 0)
      return -1;
	
	return 0;
}




Packet *
FixUSRPData::simple_action(Packet *p_)
{ 
  WritablePacket * p_in = p_->uniqueify();
//  click_chatter("packet: %s",p_in->data());
  p_in->push(1);
  char c = '1';
  memcpy(p_in->data(),&c,1);
//  click_chatter("packet: %s",p_in->data());

  //click_ether *ethh = p_in->ether_header();
  //memcpy(ethh->ether_shost, _eth.data(), 6);		  
  return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FixUSRPData)

