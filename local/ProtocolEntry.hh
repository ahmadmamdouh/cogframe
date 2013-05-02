#ifndef PROTOCOL_ENTRY_HH
#define PROTOCOL_ENTRY_HH
#include <string>


using namespace std;

struct ProtocolEntry
{
	string msg;
	long fromTimestamp, toTimestamp;
	string from_mac, to_mac;
	ProtocolEntry(){}
	ProtocolEntry(string msg, long fromTimestamp, long toTimestamp,string from_mac, string to_mac){
		this->msg = msg;
		this->fromTimestamp = fromTimestamp;
		this->toTimestamp = toTimestamp;
		this->from_mac = from_mac;
		this->to_mac = to_mac;
	}
	~ProtocolEntry(){}
};


#endif
