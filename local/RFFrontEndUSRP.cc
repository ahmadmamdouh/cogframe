#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <elements/local/RFFrontEndUSRP.hh>
#include <string>
#include "iwlib.h"
CLICK_DECLS

using namespace std;
/**
*/

RFFrontEndUSRP::RFFrontEndUSRP()
{
}

RFFrontEndUSRP::~RFFrontEndUSRP()
{
}

int
RFFrontEndUSRP::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
	       .read_mp("spectrumManager", reinterpret_cast<Element *&>(_spectrum_manager))
	      .complete() < 0)
	      return -1;
	      _spectrum_manager -> registerRFFrontEnd(this);
	return 0;
}


void
RFFrontEndUSRP::change_channel(String _if_name, uint8_t channel,String type){
click_chatter("#####RFFRONTEND_CHANNELCHANGE#####");
	void * packet_pt = malloc(sizeof(int) *2);
	WritablePacket *packet = Packet::make((void *)&packet_pt, sizeof(int) *2);
	if (packet == 0 )
		return click_chatter( "cannot make packet!");

	int control  = 0;
	int channel_info = 900000000;
//	click_chatter("#################### USRP %d", sizeof(int) *2);

	if(channel == 1)
	{
		channel_info = 900000000;
	}
	else if(channel == 6)
	{
		channel_info = 910000000;
	}
	else if(channel == 11)
	{
		channel_info = 920000000;
	}

	memcpy(packet->data(), &control, sizeof(int));
	memcpy(packet->data()+sizeof(int), &channel_info,sizeof(int) );
	output(0).push(packet);

}
void 
RFFrontEndUSRP::scan(String _ifname){
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RFFrontEndUSRP)

