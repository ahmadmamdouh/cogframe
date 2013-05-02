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
	TopologyManager() {
	}

	~TopologyManager() {
	}
	//insert a new address for me
	void insert_my_address(string HW_address)
	{
		std::transform(HW_address.begin(), HW_address.end(), HW_address.begin(), ::toupper);	
		printf("Inserted my address: %s\n", HW_address.c_str());
		
		my_addresses.insert(HW_address);
	}
	
	//insert a new address for neighbors	
	void update_neighbors(string eth) {
		std::transform(eth.begin(), eth.end(), eth.begin(), ::toupper);
		printf("Inserting neighbors: %s\n", eth.c_str());
		
		neighbors.insert(eth);
	}
	
	//insert a new address for neighbors
	void update_neighbors_ut8(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("inserted addresssssssssssssssssssssssss %s\n",mac.c_str());
		neighbors.insert(mac);
	}
	
	// insert a new primiary user address 
	void update_PUs(string eth) {
		std::transform(eth.begin(), eth.end(), eth.begin(), ::toupper);
		printf("Inserting PU: %s\n", eth.c_str());
		PUs.insert(eth);
	}
	
	// check that the given address is from my addresses
	bool my_address(string eth) {
		std::transform(eth.begin(), eth.end(), eth.begin(), ::toupper);
		return my_addresses.find(eth)!=my_addresses.end();
	}
	
	// check that the given address is from my addresses
	bool is_me(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("Is neighbor: %s\n", mac.c_str());
		
		return "FF:FF:FF:FF:FF:FF" == mac || my_addresses.find(mac) != my_addresses.end();
	}

	//check the given address is a neighbor address
	bool is_neighbor(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("Is neighbor: %s\n", mac.c_str());
		
		return neighbors.find(mac) != neighbors.end();
	}

	//check the given address is a primiary user address
	bool is_PU(uint8_t *eth) {
		string mac = convert(eth);
		std::transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
		printf("Is PU: %s\n", mac.c_str());
		
		return PUs.find(mac)!=PUs.end();
	}
	
	//return my first address	
	string getAddress(){
		return *my_addresses.begin();
	}
	
	// fill the given array with my addresses and return number of addresses i have
	int getAddress_ut8(uint8_t  source_eth[10][6])
	{
		set <string >::iterator iter_ss;
		int count=0;
		//iterate on the set of my addresses and fill the given array
		for (iter_ss = my_addresses.begin(); iter_ss != my_addresses.end(); ++iter_ss)
		 {
		      string addresss=*iter_ss;
		      char * address=addresss.c_str();
		      printf("my addressss %s \n" , address);
		      int i=0;
		      int j=0;
			    //convert address from string to uint8_t array 
		      for(i=0 ;i<6 ;i++)
		      {
		      	char hex[3];
		      	hex[0]=address[j];
		      	hex[1]=address[j+1];
		      	hex[3]="\0";
		      	printf("my part address %s\n", hex);
		      	j=j+3;
  					int d;
  					sscanf(hex,"%02x", &d);
  					source_eth[count][i]=(unsigned char)d;
		      }
		      count++;
		 }
		//make a broadcast address where it represent me also
		string str("FF:FF:FF:FF:FF:FF");
		int i=0;
    int j=0;
    //convert the broadcast address string to uint8_t array
    for(i=0 ;i<6 ;i++)
    {
    	char *hex=str[j]+""+str[j+1];
    	j=j+3;
			int d;
			sscanf(hex,"%02x", &d);
			source_eth[count][i]=(unsigned char)d;
    }
		      
		  return my_addresses.size();
	}
};
#endif
