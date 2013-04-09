#include <click/hashmap.hh>
#include <click/ipaddress.hh>
#include <iostream>
#include <map>

#ifndef _MOBILITY
#define _MOBILITY

using namespace std;

class EtherAddress;
                                                                              
class LocationEntry {

public:
	string identifier;      // IP address of this destination

	uint32_t neighbor_lat;			// Sender's Latitude.
	uint32_t neighbor_long;			// Sender's Longitude.
	EtherAddress*   neighbor_eth; // hardware address of next hop

	LocationEntry(const string &nidentifier,
		  uint32_t nlat, uint32_t nlong) :
	  identifier(nidentifier), neighbor_lat(nlat), neighbor_long(nlong)
	{}

	LocationEntry() {}
	~LocationEntry() {}
};

class IMobilityManager
{
public:
	map<string, LocationEntry> _ltable;
	
	IMobilityManager(){}
	
	~IMobilityManager(){}

	LocationEntry get_sensed_location(string identifier)
	{
		LocationEntry ret = _ltable[identifier];
		printf("Retriving location entry: %s, %d, %d\n", identifier.c_str(), ret.neighbor_lat, ret.neighbor_long);
		return ret;
	}
	
	void insert_location(string identifier, int x, int y){
		printf("New location entry added: %s, %d, %d\n", identifier.c_str(), x, y);

		LocationEntry le(identifier, x, y);
		_ltable[identifier] = le;
	}
    
}; 

#endif
