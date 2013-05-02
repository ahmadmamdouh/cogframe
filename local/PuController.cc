#include <unistd.h>
#include <pthread.h>
#include <click/config.h>
#include <elements/local/PuController.hh>
#include <elements/local/ProbabilisticDistributionFactory.cc>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
using namespace std;
CLICK_DECLS


PuController::PuController() {
}

PuController::~PuController() {
}

struct Thread_args{
	ProbabilisticDistribution *active, *inactive;
	int *can_send;
	int *time_to_live;
	TimedSource* ts;
};

void *print_message(void *arg){
		struct Thread_args *args = (struct Thread_args *)arg;
		int *can_send = args->can_send;
		int *time_to_live = args->time_to_live;
		TimedSource* ts = args->ts;
		while(true){
    		printf("Threading\n");
    		
    		*can_send = 1 - *can_send;
    		
    		if(*can_send == 0) {
    			*time_to_live = args->inactive->getTime();
   				HandlerCall::call_write(ts,"active","false");
    		} else {
    			*time_to_live = args->active->getTime();
					HandlerCall::call_write(ts,"active","true");
    		}
    		printf("Time to sleep: %d\n", *time_to_live);
    		sleep(*time_to_live);
    }
}


int
PuController::configure(Vector<String> &conf, ErrorHandler * errh)
{

	if (Args(conf, this, errh)
	      .read("ACTIVE", _active)
	      .read("INACTIVE", _inactive)	      	      
				.read_mp("TimedSource", reinterpret_cast<Element *&>(ts))
	      .complete() < 0)
	      return -1;
	      
	can_send = 0;
	      
	printf("ACTIVE: %s, INACTIVE: %s\n",_active.mutable_c_str(),_inactive.mutable_c_str()); 
	char* active_str = _active.mutable_c_str();
	char* inactive_str = _inactive.mutable_c_str();
	String active_type,inactive_type;

	while (*active_str != ' '){
		printf("%c\n", *active_str);
		active_type.append(active_str,1);
		active_str++;
	}
	active_str++;
	
	while (*inactive_str != ' '){
		inactive_type.append(inactive_str,1);
		inactive_str++;
	}
	inactive_str++;
	
	ProbabilisticDistributionFactory factory;
	ProbabilisticDistribution *active = factory.getDistribution(active_type, active_str);
	ProbabilisticDistribution *inactive = factory.getDistribution(inactive_type, inactive_str);
	time_to_live = -1;
	
	struct Thread_args *args = new Thread_args();
	args->active = active;
	args->inactive = inactive;
	args->can_send = &can_send;
	args->time_to_live = &time_to_live;
	args->ts = ts;
	printf("value: %d, address: %d\n", can_send, &can_send);
	
  pthread_t t1;
  pthread_create(&t1, NULL, &print_message, (void *) args);	  
  return 0;
}


Packet *
PuController::simple_action(Packet *p_in) {
  if (time_to_live != -1) { // annotate packet with time to live
    memcpy(p_in->data()+50, &time_to_live, sizeof(time_to_live));
  	time_to_live = -1;
  }
//  return can_send ? p_in : 0;
	return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PuController)

