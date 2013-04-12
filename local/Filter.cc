#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <elements/local/Filter.hh>
#include <elements/local/CogHeader.hh>

using namespace std;
CLICK_DECLS

Filter::Filter() {
	// TODO Auto-generated constructor stub

}

Filter::~Filter() {
	// TODO Auto-generated destructor stub
}

Packet *
Filter::simple_action(Packet *p_in) {
//64   31   50    4   93 e4

	WritablePacket * p = p_in->uniqueify();
	click_ether *ethh = p_in->ether_header();
	unsigned short cogtype=0x1234;
	//check it is a cog frame packet or not
	if(ethh->ether_type==cogtype)
	{
		uint8_t* source_address;
		struct CogHeader * format = (struct CogHeader *) (p->data()+14);
		printf("format \n");
		int type = format->type;
		if (type == 1){
			printf("COGHEADER!!!\n");
			return p_in;
		}
	}else{	
		uint8_t source_address[6];
		memcpy(source_address, ethh->ether_shost, 6);

		uint8_t dist_address[6];
		memcpy(dist_address, ethh->ether_dhost, 6);

		if(Controller::getInstance().is_me(dist_address)){
			if (Controller::getInstance().is_neighbor(source_address)) {
				printf("\n .......................passed..................... \n");
				return p_in;
			}else if(Controller::getInstance().is_PU(source_address)){
				printf("\n PU detecteddddddddddddddddddddddd \n");
				output(0).push(p_in);
			}
		}
	}
		
	
	printf("\n Didn't pass \n");
	return 0;

}
CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(Filter)
