#include <click/config.h>
#include <elements/local/PuCaller.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/handlercall.hh>
#include "iwlib.h"
#include <elements/local/Controller.cc>
#include <elements/local/Utilities.hh>
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
	printf("___________________________________________________TTL BEFORE SLEEP: %d\n",args->time_to_live);
	sleep(args->time_to_live);
	*(args->_count) = 0;
	printf("___________________________________________________PU UNSENSED!!!!!!!!\n");
	printf("___________________________________________________TTL AFTER SLEEP: %d\n",args->time_to_live);
//	int channel = Controller::getInstance().getCurrentChannel("wlan0");
	int channel = 1;
	printf("primary_user_inactive %d\n",channel);
	char buffer[100];
	int n = sprintf (buffer, "pu_unsensed %d\0",channel);
	HandlerCall::call_write(buffer,args->_llr, 0);
}


Packet *
PuCaller::simple_action(Packet *p_in)
{
  if(_count == 0)	{
		uint8_t time_to_live;
		printf("BEFORE MEMCPY %d\n",time_to_live);
		memcpy(&time_to_live, p_in->data() + 50, sizeof(time_to_live));
		printf("_________________________________________________TTL after memcpy: %d\n",time_to_live);
		struct timeval start;
		long mtime, seconds, useconds;    
		gettimeofday(&start, NULL);
		seconds  = start.tv_sec; // seconds since epoch
		useconds = start.tv_usec; // microSeconds since epoch
		mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
		click_ether *ethh = p_in->ether_header();
		uint8_t src_address[6];
		memcpy(src_address, ethh->ether_shost, 6);
		string pu_mac = Utilities::convert_uint8_to_string(src_address);
		
		struct Thread_args *args = new Thread_args();
		args->time_to_live = time_to_live;
		args->_count = &_count;
		args->_llr = _llr;
		pthread_t t1;
		pthread_create(&t1, NULL, &wait_till_PU_exit, (void *) args);	  	
		printf("Primary_User_Appeared\n");
	//	int channel = Controller::getInstance().getCurrentChannel();
		int channel =1;
		Controller::getInstance().addToPUActiveTable(pu_mac,mtime,channel,time_to_live);
		printf("primary_user_active %d\n",channel);
		char buffer[100];
		int n = sprintf (buffer, "pu_sensed %d\0",channel);
		HandlerCall::call_write(buffer,_llr, 0);
		_count = 1;
		
  }
  return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PuCaller)

