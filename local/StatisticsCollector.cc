#include <click/config.h>
#include <elements/local/StatisticsCollector.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include "iwlib.h"
#include <elements/local/Controller.cc>


CLICK_DECLS


StatisticsCollector::StatisticsCollector()
{
}

StatisticsCollector::~StatisticsCollector()
{
}

int
StatisticsCollector::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
		.read_mp("type", type) 
	    .complete() < 0)
	      return -1;
	count = 0;
		      
	return 0;
}

Packet *
StatisticsCollector::simple_action(Packet *p_in)
{
	// Calculating current timestamp milliseconds
	struct timeval start;
	long mtime, seconds, useconds;    
	gettimeofday(&start, NULL);
	seconds  = start.tv_sec; // seconds since epoch
	useconds = start.tv_usec; // microSeconds since epoch
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	
	//printf("Elapsed time: %lld milliseconds\n", mtime);
	long timestamp = mtime;
	printf("\nCopy!!\n");
	uint32_t id;
		memcpy(&id, p_in->data()+50, sizeof(id));
	printf("Done Copy!!: %d\n",id);
	// Sent packets handling
	if(type == "S") {
		// to_mac
		click_ether *ethh = p_in->ether_header();
		uint8_t dest_address[6];
		memcpy(dest_address, ethh->ether_dhost, 6);
		string to_mac = Utilities::convert_uint8_to_string(dest_address);
		// controller call
		Controller::getInstance().addToSentTable(id, timestamp, to_mac);
	} else if(type == "R") {
		Controller::getInstance().addToReceivedTable(id,timestamp);
	}
	p_in->set_anno_u32(0,5);
   return p_in;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(StatisticsCollector)

