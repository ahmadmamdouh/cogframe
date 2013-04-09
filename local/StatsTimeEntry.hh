#ifndef STATS_TIME_ENTRY_HH
#define STATS_TIME_ENTRY_HH
#include <string>


using namespace std;

struct StatsTimeEntry
{
	string ifName;
	long timestamp;
	long switchTime;
	int fromChannel;
	int toChannel;
	
	StatsTimeEntry(){}
	StatsTimeEntry(string ifName, long timestamp,long switchTime, int fromChannel, int toChannel){
		this->ifName = ifName;
		this->timestamp = timestamp;
		this->switchTime = switchTime;
		this->fromChannel = fromChannel;
		this->toChannel = toChannel;
	}
	~StatsTimeEntry(){}
};


#endif
