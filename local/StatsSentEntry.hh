#ifndef STATS_SENT_ENTRY_HH
#define STATS_SENT_ENTRY_HH
#include <string>


using namespace std;

struct StatsSentEntry
{
	uint32_t id;
	long timestamp;
	string to_mac;

	StatsSentEntry(){}
	StatsSentEntry(uint32_t id, long timestamp, string to_mac){
		this->id = id;
		this->timestamp = timestamp;
		this->to_mac = to_mac;
	}
	~StatsSentEntry(){}
};


#endif
