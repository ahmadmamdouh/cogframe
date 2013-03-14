#include <click/config.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrouter.hh>
#include <elements/local/launchlockrequester.hh>
#include <elements/local/launchrtrhandler.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <unistd.h>
CLICK_DECLS


LaunchRTRHandler::LaunchRTRHandler()
{
}

LaunchRTRHandler::~LaunchRTRHandler()
{
}

int
LaunchRTRHandler::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
      .read_mp("ROUTER", reinterpret_cast<Element *&>(_router))
      .complete() < 0)
      return -1;

	return 0;
}




Packet *
LaunchRTRHandler::simple_action(Packet *p_in)
{
		_router->set_rtr_received_positive();
   return 0;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(LaunchRTRHandler)
