
#include <click/config.h>
#include <elements/local/StatisticsCollector.hh>
#include <click/glue.hh>
#include <click/args.hh>
#include <click/handlercall.hh>
#include <click/confparse.hh>

CLICK_DECLS

StatisticsCollector::StatisticsCollector()
{
}

StatisticsCollector::~StatisticsCollector()
{
}

int
StatisticsCollector::initialize(ErrorHandler *)
{
    _count = 0;
    return 0;
}

int
StatisticsCollector::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
	  .read_mp("TimedSource", reinterpret_cast<Element *&> (_ts))
	  .read_mp("StatPacketSender", reinterpret_cast<Element *&> (_sender))
      .complete() < 0)
      return -1;

	return 0;
}

inline Packet *
StatisticsCollector::simple_action(Packet *p)
{
        _count++;
	p->set_user_anno_u(1,0);
	click_chatter("STATISTICS COLLECTOR YAY::  %d", _count);
	/*if (_count == 50){
	  int success = (*_ts).setActive(false);
	  click_chatter("Change Active parameter %d", success);
	  (*_sender).sendMessage();
	}
	*/
	return p;
}



CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(StatisticsCollector)
