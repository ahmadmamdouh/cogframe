#ifndef Launch_Router_HH
#define Launch_Router_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/ipaddress.hh>
#include <click/etheraddress.hh>
#include <clicknet/ether.h>
#include <elements/local/launch.hh>
#include <elements/local/launchrequester.hh>
#include <elements/local/launchlockrequester.hh>
#include <click/confparse.hh>
#include <click/timer.hh>
#include <click/hashmap.hh>
#include <elements/local/launchrtrsender.hh>
#include <list>		// list class library
#include <elements/local/Controller.cc>
using namespace std;
CLICK_DECLS

class LaunchRouter : public Element { public:

	LaunchRouter();
	~LaunchRouter();

	const char *class_name() const		{ return "LaunchRouter"; }
	const char *port_count() const		{ return PORTS_1_1; }
	const char *processing() const		{ return AGNOSTIC; }
	
	int configure(Vector<String> &, ErrorHandler *);
	int initialize(ErrorHandler *);
	
	Packet * simple_action(Packet *p_in);

	void insert_route(const IPAddress &nip,
	      uint32_t nlat, uint32_t nlong, 
	       uint8_t * ne, int chls_size, uint8_t* channels_id, float* channels_pu_prob, uint32_t swt);
	void update_route(const IPAddress &nip, int chls_size, uint8_t* channels_id, float* channels_pu_prob);
	
	void cant_use_channel(int channel_number);
	
	void set_channel_loc_positive();
	void set_channel_loc_negative();
	

	void set_ready_tosend_positive();
	void set_rtr_received_positive();

	void set_ready_for_another_packet_negative();

	int _already_has_lock;
	
  uint8_t channel_used ;

private:
	int counter;
	
	list<Packet *> packets_holded;

	Packet * _holded_packet;
	IPAddress _dst_ip;

	IPAddress _ip;
	EtherAddress _ether_address_eth;
	uint8_t _eth [6];

	bool _channel_lock_positive;
	class IPAddress locked_neighbor_ip;

	bool _routingtable_available;

	bool _rtr_received;

	bool _ready_for_another_packet;


	bool _ready_tosend;
	uint32_t my_lat;			// this node's Latitude.
	uint32_t my_long;			// this node's Longitude.


	/* destination node's addresses */
	IPAddress dst_ip;
	EtherAddress dst_eth;

	uint32_t dst_lat;			// this node's Latitude.
	uint32_t dst_long;			// this node's Longitude.

	
	/* timers 
	1. _respone_waiting_timer for waiting on responses to reach this node from its nieghbors
	2. _lock_waiting_timer for waiting on channel lock to reach this node from the node it chose as the next hop
	3. _routing_table_entry_timer for making route table expire */
	Timer _respone_waiting_timer;
	Timer _lock_waiting_timer;
	Timer _rtr_waiting_timer;
	Timer _routing_table_entry_timer;

	uint32_t _repsonse_waiting_ms;
	uint32_t _lock_waiting_ms;	
	uint32_t _routing_table_entry_ms;


	static void static_use_responses(Timer *, void *e) { ((LaunchRouter *) e)->use_responses(); }
	void use_responses();

	static void static_use_lock(Timer *, void *e) { ((LaunchRouter *) e)->use_lock(); }
	void use_lock();

	static void static_make_routetable_expire(Timer *, void *e) { ((LaunchRouter *) e)->make_routetable_expire(); }
	void make_routetable_expire();

	static void static_use_rtr(Timer *, void *e) { ((LaunchRouter *) e)->use_rtr(); }
	void use_rtr();
	

	LaunchCtrlRequester *_requester;
	LaunchLockRequester * _lock_requester;
	LaunchRTRSender * _rtr_sender;
  
	//Metric Calculation Function

	double distance(double lat1, double lon1, double lat2, double lon2, char unit);
	double deg2rad(double deg);
	double rad2deg(double deg);

	//Routing Table Entry

	class RouteEntry {

	public:
		class IPAddress  neighbor_ip;      // IP address of this destination

		uint32_t neighbor_lat;			// Sender's Latitude.
		uint32_t neighbor_long;			// Sender's Longitude.

		uint8_t   neighbor_eth[6]; // hardware address of next hop

		map<int, struct Channel> *channels;

		uint32_t switching_time;
		uint8_t channel_used;

		RouteEntry(const IPAddress &nip,
			  uint32_t nlat, uint32_t nlong, 
			   uint8_t * ne, int channels_size, uint8_t* chls_id, float* chls_pu_prob,
			   	uint32_t swt) {
		 			 neighbor_ip = nip;
		 			 neighbor_lat = nlat;
		 			 neighbor_long = nlong;
		 			 switching_time = swt; 
		 			 channels = new map<int, struct Channel>();
					 memcpy(neighbor_eth,ne,6);
					 for(int i = 0; i < channels_size; i++) {
					 	struct Channel ch (chls_id[i], chls_pu_prob[i], false);
						(*channels)[chls_id[i]] = ch; 
					}
		}

		RouteEntry() {}
		~RouteEntry() {}
	};



	typedef HashMap<IPAddress, RouteEntry> RTable;
	typedef RTable::const_iterator RTIter;
	RTable _rtes;
	
	double calculate_metric(RouteEntry r, LocationEntry l_dst, LocationEntry l_neighbor);
	
	RouteEntry choose_bestneighbor(IPAddress _current_dst_addr, HashMap<IPAddress, RouteEntry>  _rtes) {
	Controller controller = Controller::getInstance();
		click_chatter("ENTERED CHOOSING BEST NEIGHBOR!!\n");
		vector<int> *channel_ids = controller.get_channels();

		if(_rtes.findp(_current_dst_addr) != 0) {
			printf("STILL HERE \n");
			click_chatter("DST_ADDR_IP %s\n",_current_dst_addr.unparse().c_str());
			RouteEntry * editable = _rtes.findp(_current_dst_addr);
			editable->neighbor_ip.unparse().c_str();
			
			float min_pu_prob = 1 << 28;
			int new_channel_id = -1;
			
			for (int i = 0; i < (*channel_ids).size(); i++) {
				int channel_id = (*channel_ids)[i];
				map<int, struct Channel> *route_channels = editable->channels;
				float current_pu_prob = (*route_channels)[channel_id].pu_prob;
				if (!controller.is_pu_active(channel_id)) {
					if (current_pu_prob < min_pu_prob) {
						min_pu_prob = current_pu_prob;
						new_channel_id = channel_id;
					}
				}
			}
			
			if (new_channel_id == -1) {
				printf("No Channel found to use . R.I.P");
				exit(0);
			}
			editable->channel_used = new_channel_id;
			printf("New Neighbour choice %d\n", new_channel_id);
			click_chatter("EDITABLE_IP %s\n",(*editable).neighbor_ip.unparse().c_str()); 
			return *editable;
		}
		
		click_chatter("Start debugging..........\n");
		
		double last_metric = 10000;
		IPAddress best_ip;
		double current_metric;

		for (RTIter iter = _rtes.begin(); iter.live(); iter++) {
			RouteEntry rte = iter.value();
			RouteEntry * editable = _rtes.findp(rte.neighbor_ip);
			
			LocationEntry lentry_dst =  controller.get_sensed_location(string(_current_dst_addr.unparse().mutable_c_str()));//rte.neighbor_ip);
			
			//click_chatter("%d %d %d %d %d %d %d %d", rte.neighbor_lat, rte.neighbor_long, rte.channel, rte.pu_behavior, rte.channel1, rte.pu_behavior1, rte.channel2, rte.pu_behavior2);
			
			click_chatter("Sensed .......... \n");
			LocationEntry lentry_neighbor =  controller.get_sensed_location(string(rte.neighbor_ip.unparse().mutable_c_str()));
			click_chatter("Sensed  neighbor.......... \n");
			
			float min_pu_prob = 1 << 28;
			int new_channel_id = -1;
			
			for (int i = 0; i < (*channel_ids).size(); i++) {
				int channel_id = (*channel_ids)[i];
				map<int, struct Channel> *route_channels = editable->channels;
				float current_pu_prob = (*route_channels)[channel_id].pu_prob;
				if (!controller.is_pu_active(channel_id)) {
					if (current_pu_prob < min_pu_prob) {
						min_pu_prob = current_pu_prob;
						new_channel_id = channel_id;
					}
				}
			}
			
			if (new_channel_id != -1) {
				rte.channel_used = new_channel_id;
				editable->channel_used = new_channel_id;
				current_metric = calculate_metric(rte,lentry_dst, lentry_neighbor);
			  click_chatter("currentMetric ... %.f\n", current_metric); 
			  printf("CALCULATING DISTANCE BETWEEN %s AND %s AND ITIS: %f ON CHANNEL %d \n", rte.neighbor_ip.unparse().c_str(),
			  lentry_dst.identifier.c_str() ,current_metric, rte.channel_used);
			
				if(current_metric < last_metric) {
					click_chatter("bestIP modified.......... \n"); 
					last_metric = current_metric;
					best_ip = rte.neighbor_ip;
				}	
				
			} else {
				click_chatter("error.......... \n"); 
				rte.channel_used = 0;
			}
		}
		
		click_chatter("Method ended.......... \n"); 
		return *_rtes.findp(best_ip);
	};

};
CLICK_ENDDECLS
#endif
