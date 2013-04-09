#ifndef STATS_RECEIVED_ENTRY_HH
#define STATS_RECEIVED_ENTRY_HH
#include <string>


using namespace std;

struct StatsReceivedEntry
{
	uint32_t id;
	long timestamp;

	StatsReceivedEntry(){}
	StatsReceivedEntry(uint32_t id, long timestamp){
		this->id = id;
		this->timestamp = timestamp;
	}
	~StatsReceivedEntry(){}
};


#endif
