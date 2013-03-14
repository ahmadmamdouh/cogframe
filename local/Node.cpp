#include "Node.h"
Node :: Node(eth,ip,lat,lon,pu0,pu1,pu2,s0,s1,s2)
{
	_my_eth_addr=eth;		//ehternet HW address
	_my_ip=ip;

	_my_lat=lat;			// my Latitude.        
    _my_long=lon;			// my Longitude.
	
	// PU behavior of the three candidates channels
	_pu_behavior[0]=pu0;
	_pu_behavior[1]=pu1;
	_pu_behavior[2]=pu2;

	// Strength  of the three candidates channels
	 _channel_strength[0]=s0;
	 _channel_strength[1]=s1;
	 _channel_strength[2]=s2;

}; 


Node :: ~Node(){}; 

