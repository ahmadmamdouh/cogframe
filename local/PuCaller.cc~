#include <click/config.h>
#include <elements/local/PuCaller.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/handlercall.hh>
#include "iwlib.h"
CLICK_DECLS



PuCaller::PuCaller() {
}

PuCaller::~PuCaller() {
}

int
PuCaller::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_mp("LAUNCH_RESPONDER", reinterpret_cast<Element *&>(_llr)) // interface name that will switch the channel.
	      .complete() < 0)
	      return -1;
	_count = 0;
	click_chatter("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= CONFIGURE??");
	return 0;
}




Packet *
PuCaller::simple_action(Packet *p_in)
{
	if(_count==0)	{
		printf("Primary_User_Appeared\n");
		HandlerCall::call_write("pu_sensed 1",_llr, 0);
		_count = 1;
	}
  return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PuCaller)

