#include <elements/local/ISSensor.cc>
#include <elements/local/IMobilityManager.cc>
#include <elements/local/TopologyManager.cc>
#include <elements/local/Channel.hh>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <map>

#ifndef _CONTROLLER
#define _CONTROLLER

using namespace std;

class Controller {
private:
	map<string, string> macToIP, IPToMac;

	ISSensor sSensor;
	IMobilityManager mobilityManager;
	TopologyManager TManager;
	map<int, struct Channel> channels;

	int identifierType(string identifier){
		if(macToIP.count(identifier))return 1;//MAC.
		else if(IPToMac.count(identifier))return 2;//IP.
		else return 0; //Invalid.
	}	

	Controller() {
		//Read the information from the module file.
		freopen("elements/local/ModuleFile.txt", "r", stdin);
		//ifstream fin("ModuleFile.txt");		
		printf("Controller started!!!!!!!!!\n");

		string mobility = "...", topology = "...";
		int numberOfNodes = -1, numberOfEdges = 0, numberofAddresses = 0,
				num_PUs = 0;

		//Name and type
		string name = "..", type = "...";
		cin >> name;
		cin >> type;
		printf("Node name: %s, type: %s\n", name.c_str(), type.c_str());		
		
		//is_Primary user
		string ACTIVE_DIST = "...", INACTIVE_DIST = "...";
		if (type == "Primary") {
			cin >> ACTIVE_DIST;
			cin >> INACTIVE_DIST;
		}

		//My ethernet addresses
		cin >> numberofAddresses;
		printf("Num Addresses: %d\n",numberofAddresses);
		for (int e = 0; e < numberofAddresses; e++) {
			string address = "...", IP = "..";
			cin >> address >> IP;
			char * buffer = (char *) address.c_str();
			TManager.insert_my_address(address);
		}

		//My WiFi addresses
		cin >> numberofAddresses;

		for (int e = 0; e < numberofAddresses; e++) {
			string address = "...", IP = "..";
			cin >> address >> IP;
			char * buffer = (char *) address.c_str();

			TManager.insert_my_address(address);
		}

		//channels probabilities && Spectrum Sensing.
		sSensor = ISSensor();
		string ip;
		int numberOfChannels;

		cin >> numberOfChannels;

		for (int c = 0; c < numberOfChannels; c++) {
			int channel = 0;
			int probability = 0; // Change to float 
			cin >> channel >> probability;
			struct Channel ch(channel, probability, false);
			channels[channel] = ch;
		}
	
		
		for(set<string>::iterator myIterator = TManager.my_addresses.begin();
		    myIterator != TManager.my_addresses.end();
		    myIterator++){
		    
			sSensor.insert_route_entry(*myIterator, channels[1].id, channels[1].pu_prob, channels[6].id, channels[6].pu_prob, channels[11].id, channels[11].pu_prob);
		}
//		printf("My channels: %s, %d: %d, %d: %d, %d %d\n", ip.c_str(), channels[0], probabilites[0], channels[1], probabilites[1], channels[2], probabilites[2]);

		//My location
		string my_lat = "..", my_long = "..";
		cin >> my_lat >> my_long; //Do Nothing
		printf("My location: %s, %s\n", my_lat.c_str(), my_long.c_str());

		//Options for mobility and topology
		string mobilityOption = "..", topologyOption = "..";
		cin >> mobilityOption;
		cin >> topologyOption;
		printf("MobilityOption: %s, TopologyOption: %s\n", mobilityOption.c_str(), topologyOption.c_str());
		
		//if (topologyOption == "LOCATION_BASED") { ////// NOT NOW.	
		//	string radius = "";
		//	cin >> radius;
		//}
		
		//Overall conversions.
		int numberOfConversion = 0;
		cin >> numberOfConversion;
		for(int k = 0; k < numberOfConversion; k++){
			string from, to;
			cin >> from >> to;
			
			macToIP[from] = to;
			IPToMac[to] = from;
			
			printf("Mac: %s -> IP: %s\n", from.c_str(), to.c_str());
		}
		

		//Overall Topology Manager.
		cin >> numberOfEdges;

		for (int e = 0; e < numberOfEdges; e++) {
			string node1, node2;
			cin >> node1 >> node2;
			char * buffer = (char *) node1.c_str();

			if (TManager.my_address(node1)) {
				TManager.update_neighbors(node2);
			}
		}
		
		cin >> numberOfEdges;
		for (int e = 0; e < numberOfEdges; e++) {
			string node1, node2;
			cin >> node1 >> node2;
			char * buffer = (char *) node1.c_str();

			if (TManager.my_address(node1)) {
				TManager.update_neighbors(node2);
			}
		}

		//Overall Mobility Manager.
		mobilityManager = IMobilityManager();

		cin >> numberOfNodes;
		printf("number of nodes %s %d\n", mobility.c_str(), numberOfNodes);
		for (int i = 0; i < numberOfNodes; i++) {
			string mac;
			int x, y;
			cin >> mac >> x >> y;

			mobilityManager.insert_location(mac, x, y);
		}

		//PUs
		cin >> num_PUs;
		for (int e = 0; e < num_PUs; e++) {
			string address;
			cin >> address;
			
			TManager.update_PUs(address);
		}
		
		printf("Controller ended parsing the module file\n");
	}
	
  void operator=(Controller const&); // Don't implement
	
public:
	static Controller& getInstance(){
		
		static Controller instance;
		
		return instance;
	}
	
	LocationEntry get_sensed_location(string identifier) {
		printf("Retriving location entry (Unknown identifier): %s\n", identifier.c_str());
		
		int type = identifierType(identifier);
		string mac = type == 1 ? identifier : ( type == 2 ? IPToMac[identifier] : 0);
		return mobilityManager.get_sensed_location(mac);
	}

	RouteEntry get_sensed_Channel(string identifier) {
		printf("Retriving channel entry (Unknown identifier): %s\n", identifier.c_str());	
		
		int type = identifierType(identifier);
		string mac = type == 1 ? identifier : ( type == 2 ? IPToMac[identifier] : 0);
		return sSensor.get_Sensed_Channel(mac);
	}

	bool is_neighbor(uint8_t *eth) {
		return TManager.is_neighbor(eth);
	}
	
	bool is_me(uint8_t *eth) {
		return TManager.is_me(eth);
	}
	
	bool is_PU(uint8_t *eth) {
		return TManager.is_PU(eth);
	}
	
	string getAddress(){
		return TManager.getAddress();
	}
	
	// Channel Interaction
	
	void set_pu_active(int id) {
		channels[id].pu_active = true;
	}
	
	void set_pu_inactive(int id) {
		channels[id].pu_active = false;
	}
	
	void set_pu_prob(int id, float prob) {
	  channels[id].pu_prob = prob;
	}
	
	bool is_pu_active(int id) {
		return channels[id].pu_active;
	}
	
	float get_pu_prob(int id) {
		return channels[id].pu_prob;
	}
	
	
};

#endif
