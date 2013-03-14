#include <click/ipaddress.hh>
#include <click/hashmap.hh>
#include <iostream>
#include <map>

#ifndef _SENSOR
#define _SENSOR
                                                                              
using namespace std;							
class RouteEntry {

public:
	string identifier;      // IP address of this destination

	uint8_t   neighbor_eth[6]; // hardware address of next hop

	uint8_t channel;
	uint32_t pu_behavior;
	
	uint8_t channel1;
	uint32_t pu_behavior1;

	uint8_t channel2;
	uint32_t pu_behavior2;
	

	uint32_t switching_time;
	uint8_t channel_used;


	RouteEntry(const string &nidentifier,
		   uint8_t * ne, 	uint8_t chl, uint32_t pu,uint8_t chl1,uint32_t pu1,uint8_t chl2,uint32_t pu2,
				uint32_t swt) :
	  identifier(nidentifier), 
	  channel(chl),  pu_behavior(pu),channel1(chl1),  pu_behavior1(pu1),channel2(chl2),  pu_behavior2(pu2), switching_time(swt)
	{memcpy(neighbor_eth,ne,6);}

	RouteEntry() {}
	~RouteEntry() {}
};

class ISSensor
{
  public:
	map<string, RouteEntry> _rtes;
    /**
        Gets the strength of the channel specified by its number
		;for more than three channels that would be good the 
		parameters could be the node HW address to look it up in the table.
                                      
                                                                              */
	ISSensor(){}							
					
    RouteEntry get_Sensed_Channel(string identifier){
    		RouteEntry ret = _rtes[identifier];
    		click_chatter("Retriving route entry: %s, (%d, %d), (%d, %d), (%d, %d)\n", identifier.c_str(), 
	ret.channel,
	ret.pu_behavior,
	ret.channel1,
	ret.pu_behavior1,
	ret.channel2,
	ret.pu_behavior2);
		return ret;
    }

	void insert_route_entry(string identifier, int ch1, int p1, int ch2, int p2, int ch3, int p3){
		click_chatter("New route entry added: %s, (%d, %d), (%d, %d), (%d, %d)\n", identifier.c_str(), ch1, p1, ch2, p2, ch3, p3);
		int switching_time = 1;
		
		uint8_t source_address[6];
		RouteEntry re1(identifier, source_address, ch1, p1, ch2, p2, ch3, p3, switching_time);
		_rtes[identifier] = re1;
	}
};

#endif
