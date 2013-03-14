#ifndef NODE_HH
#define NODE_HH
class Node{ 

	public: 

	uint32_t _my_lat;			// my Latitude.        
    uint32_t _my_long;			// my Longitude.
	
	// PU behavior of the three candidates channels
	double _pu_behavior[3];

	// Strength  of the three candidates channels
	double _channel_strength[3];


	string _my_eth;	
	string _my_ip;


	Node(); 
	~Node(); 


}; 
#endif
