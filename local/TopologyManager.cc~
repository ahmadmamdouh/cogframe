#include <algorithm>
#include <string>
#include <click/string.hh>
#include <vector>
#include <click/hashmap.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <set>
#include <iostream>

#ifndef _TOPOLOGYMANAGER
#define _TOPOLOGYMANAGER

using namespace std;

class TopologyManager {
public:
	set<string> my_addresses;//Ethernet and wireless mixed.
	//vector<PUBehavior> primary_users;
	set<string> neighbors;
	set<string> PUs;
	int my_ether_len;
	//convert uint8_t * to String
	string convert(uint8_t *eth) {
		char buffer[100];
		sprintf(buffer, "%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x\0", eth[0], eth[1],
				eth[2], eth[3], eth[4], eth[5]);
		return string(buffer);
	}

//	String convert_str_TO_Str(string str)
//	{
//		char * buffer=(char *)str.c_str();
//		my_ether_len = 0;
//		while (buffer[my_ether_len++])
//			;
//		String add;
//		add.append(buffer, my_ether_len);
//		//printf("ay 7aga tany str to str  %s \n", buffer);
//		return add;
	//}
	TopologyManager() {//hard coding of broadcast ether address
		//my_addresses.insert("ff:ff:ff:ff:ff:ff");
	}

	~TopologyManager() {
	}
	
	void insert_my_address(string HW_address)
	{
		std::transform(HW_address.begin(), HW_address.end(), HW_address.begin(), ::toupper);	
		printf("Inserted my address: %s\n", HW_address.c_str());
		
		my_addresses.insert(HW_address);
	}

	bool is_neighbor(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("Is neighbor: %s\n", mac.c_str());
		
		return neighbors.find(mac) != neighbors.end();
	}
	
	void update_neighbors(string eth) {
		std::transform(eth.begin(), eth.end(), eth.begin(), ::toupper);
		printf("Inserting neighbors: %s\n", eth.c_str());
		
		neighbors.insert(eth);
	}
	
	
	bool is_PU(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("Is PU: %s\n", mac.c_str());
		
		return PUs.find(mac)!=PUs.end();
	}
	
	void update_PUs(string eth) {
		std::transform(eth.begin(), eth.end(), eth.begin(), ::toupper);
		printf("Inserting PU: %s\n", eth.c_str());
		
		PUs.insert(eth);
	}
	
	bool my_address(string eth) {
		std::transform(eth.begin(), eth.end(), eth.begin(), ::toupper);
		return my_addresses.find(eth)!=my_addresses.end();
	}
	bool is_me(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("Is neighbor: %s\n", mac.c_str());
		
		return "FF:FF:FF:FF:FF:FF" == mac || my_addresses.find(mac) != my_addresses.end();
	}
	
	string getAddress(){
		return *my_addresses.begin();
	}

};
#endif
