#ifndef CogHeader_HH
#define CogHeader_HH


using namespace std;

struct CogHeader
{
	int type;
	enum Type
	{
		Discover = 1,
		EndExperiment = 2,
		COGTYPE = 0x1234,
	};
	uint8_t   source_eth[10][6];
	int number_of_addresses;
};


#endif
