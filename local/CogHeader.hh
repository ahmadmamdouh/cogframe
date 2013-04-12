#ifndef CogHeader_HH
#define CogHeader_HH
#include <click/glue.hh>
#include <click/string.hh>
#include <click/ipaddress.hh>
#include <vector>
#include <elements/local/Channel.hh>
#include <string>

#define MAX_CHANNELS 47

using namespace std;

struct CogHeader
{
	uint8_t type;
	uint8_t   source_eth[10][6];
	int number_of_addresses;
};


#endif
