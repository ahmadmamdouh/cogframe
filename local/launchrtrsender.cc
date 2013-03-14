#include <click/config.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrtrsender.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>

CLICK_DECLS

LaunchRTRSender::LaunchRTRSender()
{
}

LaunchRTRSender::~LaunchRTRSender()
{
}

int
LaunchRTRSender::configure(Vector<String> &conf, ErrorHandler *errh)
{
  
  return 0;
}

int
LaunchRTRSender::initialize(ErrorHandler *)
{
  return 0;
}


void
LaunchRTRSender::send_RTR()
{

	_lh.type = launch_ctrl_hdr::LAUNCH_READY_TORECIEVE;
	WritablePacket *packet = Packet::make((void *)&_lh, sizeof(_lh));

	if (packet == 0 )
		return click_chatter( "cannot make packet!");

	 output(0).push(packet);


	
}


CLICK_ENDDECLS
EXPORT_ELEMENT(LaunchRTRSender)
