#ifndef LAUNCH_HH
#define LAUNCH_HH
#include <click/glue.hh>
#include <click/string.hh>
#include <click/ipaddress.hh>
#include <vector>
#include <elements/local/Channel.hh>

#define MAX_CHANNELS 47

using namespace std;

struct launch_ctrl_hdr
{
	uint8_t type;
	enum Type
	{
		LAUNCH_REQ = 1,
		LAUNCH_RES = 2,
		LAUNCH_UPDATE = 3,
		LAUNCH_LOCK_REQ = 4,
		LAUNCH_LOCK_RES = 5,
		LAUNCH_READY_TORECIEVE = 6
	};
	uint32_t my_lat;			// Sender's Latitude.
	uint32_t my_long;			// Sender's Longitude.
	
	uint8_t channels_id[MAX_CHANNELS];
	float channels_pu_prob[MAX_CHANNELS];
	
	int channels_size;
	
	uint8_t channel_used;

	uint32_t switching_time;

	uint8_t lock_response;
	IPAddress neighbor_ip;		/* Neighbor response with its IP		     */
	IPAddress dst_ip;
  
};


#endif
