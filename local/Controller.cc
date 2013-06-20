#include <elements/local/ISSensor.cc>
#include <elements/local/IMobilityManager.cc>
#include <elements/local/TopologyManager.cc>
#include <elements/local/Channel.hh>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <map>
#include <elements/local/StatsSentEntry.hh>
#include <elements/local/StatsTimeEntry.hh>
#include <elements/local/ProtocolEntry.hh>
#include <elements/local/StatsReceivedEntry.hh>
#include <elements/local/DiscoverNeighbors.hh>
#include <elements/local/ProbabilisticDistributionsLoader.cc>
#ifndef _CONTROLLER_CC
#define _CONTROLLER_CC

using namespace std;

class Controller {
private:
	map<string, string> macToIP, IPToMac;
	map<string, int> currentChannels;
	DiscoverNeighbors * dn;
	ISSensor sSensor;
	string name;
	IMobilityManager mobilityManager;
	TopologyManager TManager;
	map<int, struct Channel> *channels;
	vector<int> *channels_id;
	map<int, vector<struct StatsSentEntry> > sentTable;
	map<int, vector<struct StatsTimeEntry> > switchTable;
	map<int, vector<struct StatsReceivedEntry> > receivedTable;
	map<int, vector<struct ProtocolEntry> > protocolTable;
	ProbabilisticDistributionsLoader pdl;
	map<string, vector< string > >  puActive;
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
		channels = new map<int, struct Channel>();
		channels_id = new vector<int>();
		pdl = ProbabilisticDistributionsLoader();
		pdl.addDistributions();
		string mobility = "...", topology = "...";
		int numberOfNodes = -1, numberOfEdges = 0, numberofAddresses = 0,
				num_PUs = 0;

		//Name and type
		name = "..";
		string type = "...";
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
			string address = "...", IP = "..", ifname = ""..;
			cin >>ifname >> address >> IP;
			char * buffer = (char *) address.c_str();

			TManager.insert_my_address(address);
		}

		//channels probabilities && Spectrum Sensing.
		string ip;
		int numberOfChannels;

		cin >> numberOfChannels;

		for (int c = 0; c < numberOfChannels; c++) {
			int channel = 0;
			float probability = 0; 
			cin >> channel >> probability;
			printf("%d %f\n",channel, probability);
			struct Channel ch(channel, probability, false);
			(*channels)[channel] = ch;
			(*channels_id).push_back(channel);
		}
	
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

	map<int, struct Channel>* get_sensed_Channel() {
		return channels;
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
	string getName(){
		return name;
	}
	vector <struct StatsSentEntry> getSentTable() {
		return sentTable;
	}	

	void addToSwitchTable(string ifName, long timestamp, long switchTime, int fromChannel, int toChannel,uint32_t flowNum){
		struct StatsTimeEntry ste(ifName, timestamp, switchTime, fromChannel, toChannel);
		switchTable.push_back(ste);
	}
	
	void addToReceivedTable(uint32_t id, long timestamp,uint32_t flowNum) {
		struct StatsReceivedEntry sre(id, timestamp);
		receivedTable.push_back(sre);
	}
	
	void addToSentTable(uint32_t id, long timestamp, string to_mac,uint32_t flowNum) {
		struct StatsSentEntry sse(id, timestamp, to_mac);
		if(sentTable.count[flowNum]){
			vector<struct StatsSentEntry> v;
			v.push_back(sse);
			sentTable[flowNum] = v;
		}else{
			printf("Insert in table\n");
			sentTable.push_back(sse);
			printf("Size of table: %d",sentTable.size());
		}
	}
	
	
	void addToProtocolTable(string msg, long fromTimestamp, long toTimestamp, string from_mac, string to_mac,uint32_t flowNum){
		struct ProtocolEntry pt(msg, fromTimestamp, toTimestamp, from_mac, to_mac);
		protocolTable.push_back(pt);
	}

/*
	void addToPUInactiveTable(string pumac, long timestamp, long ttl){
			pair<long,long> p;
			p = make_pair(timestamp, ttl);
			if( puInactive.count(pumac)){
				puInactive[pumac].push_back(p);
			}else{
				vector< pair<long, long> > v;
				v.push_pack(p);
				puInactive[pumac] = v;
			}			
	}
	
*/

	void addToPUActiveTable(string pumac, long timestamp,int channel, long ttl){
			char buffer[200];
			sprintf (buffer, "%ld %d %ld",timestamp, channel, ttl);
			string entry(buffer);
			if( puActive.count(pumac)){
				puActive[pumac].push_back(entry);
			}else{
				vector< string > v;
				v.push_back(entry);
				puActive[pumac] = v;
			}			
	}
	
	void printStatsFile() {
		ofstream myfile;
		string filename;
		char buffer[100];
		sprintf(buffer, "stats_%s.txt\0",name.c_str());
		filename = string(buffer);
		myfile.open(filename.c_str()); //stats_name_role
		//myfile << "Name"<<endl;
		//myfile << "Role"<<endl;
		myfile<< getAddress() <<endl;
		
		
		typedef std::map<std::string, vector<struct StatsSentEntry> >::iterator it_type;
	for(it_type iterator = m.begin(); iterator != m.end(); iterator++) {
		// iterator->first = key
		// iterator->second = value
		// Repeat if you also want to iterate through the second map.
}
		
		
		myfile<<sentTable.size()<<endl;
		for (int i = 0; i < sentTable.size(); ++i) {
			struct StatsSentEntry sse = sentTable[i];
			printf("------------------------->>>> SENT TABLE\n");
			myfile << sse.id << " "<< sse.timestamp << " " << sse.to_mac << endl;
		}
		
		// dump recieved packets table entries
		myfile<<receivedTable.size()<<endl;
		for (int i = 0; i < receivedTable.size(); ++i) {
			struct StatsReceivedEntry sre = receivedTable[i];
			printf("------------------------->>>> RECEIVED TABLE\n");
			myfile << sre.id << " "<< sre.timestamp << endl;
		}
		
		// dump switching time table entries
		myfile<<switchTable.size()<<endl;
		for (int i = 0; i < switchTable.size(); ++i) {
			struct StatsTimeEntry ste = switchTable[i];
			myfile << ste.ifName << " "<< ste.timestamp << " " <<ste.switchTime << " " <<  ste.fromChannel <<" "<<ste.toChannel << endl;
		}
		
		myfile<<protocolTable.size()<<endl;
		for (int i = 0; i < protocolTable.size(); ++i) {
			struct ProtocolEntry pt = protocolTable[i];
			myfile << pt.msg << " " << pt.from_mac << " " << pt.to_mac << " "<< pt.fromTimestamp  << " "<< pt.toTimestamp << endl;
		}
		
		myfile<< puActive.size() << endl;
		map<string, vector<string > >::const_iterator activeItr;
		for(activeItr = puActive.begin(); activeItr != puActive.end();++activeItr){
			myfile<<(*activeItr).first << endl;
			vector<string > activeVec = (*activeItr).second;
			myfile<<activeVec.size() << endl;
			for(int i = 0; i < activeVec.size(); ++i){
				myfile<<activeVec[i]<< endl;
			}
		}
		myfile.close();
		
	}
	
		
	// Channel Interaction
	void set_pu_active(int id) {
		(*channels)[id].pu_active = true;
	}
	
	void set_pu_inactive(int id) {
		(*channels)[id].pu_active = false;
	}
	
	void set_pu_prob(int id, float prob) {
	  (*channels)[id].pu_prob = prob;
	}
	
	bool is_pu_active(int id) {
		return (*channels)[id].pu_active;
	}
	
	float get_pu_prob(int id) {
		return (*channels)[id].pu_prob;
	}
	
	vector<int>* get_channels() {
		return channels_id;
	}
	void update_neighbors_ut8(uint8_t *eth){
		 TManager.update_neighbors_ut8(eth);
	}
	int getAddress_ut8(uint8_t  source_eth [10][6])
	{
		return TManager.getAddress_ut8(source_eth);
	}
	
	void setCurrentChannel(string ifName, int channel){
			currentChannels[ifName] = channel;
	}
	
	int getCurrentChannel(string ifName){
		return currentChannels[ifName];
	}
	
	void set_discoverneighbors(DiscoverNeighbors *const DN)
	{
		dn=DN;
	}
	void discover ()
	{
		dn->discover();
	}
	
	
};

#endif
