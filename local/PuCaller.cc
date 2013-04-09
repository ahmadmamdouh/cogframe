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
		.read_mp("LAUNCH_RESPONDER", reinterpret_cast<Element *&>(_llr))
	      .complete() < 0)
	      return -1;
	_count = 0;
	return 0;
}

struct Thread_args{
	int time_to_live;
	int *_count;
	LaunchLockResponder* _llr;
};

void *wait_till_PU_exit(void *arg){
	struct Thread_args *args = (struct Thread_args *)arg;
	sleep(args->time_to_live);
	*(args->_count) = 0;
	HandlerCall::call_write("pu_unsensed 1",args->_llr, 0);
}


Packet *
PuCaller::simple_action(Packet *p_in)
{
  if(_count == 0)	{
    int time_to_live;
    memcpy(&time_to_live, p_in->data()+22, sizeof(time_to_live));
    struct Thread_args *args = new Thread_args();
    args->time_to_live = time_to_live;
    args->_count = &_count;
    args->_llr = _llr;
    pthread_t t1;
    pthread_create(&t1, NULL, &wait_till_PU_exit, (void *) args);	  
	printf("Primary_User_Appeared\n");
	HandlerCall::call_write("pu_sensed 1",_llr, 0);
	_count = 1;
  }
  return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PuCaller)

