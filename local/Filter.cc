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
Filter ::Filter(){
	disc=false;
}
Filter::~Filter() {
	// TODO Auto-generated destructor stub	
}

int
Filter::configure(Vector<String> &conf, ErrorHandler *errh)
{
   
	if (Args(conf, this, errh)
      .complete() < 0)
      return -1;

	return 0;
}

int
Filter::initialize(ErrorHandler *)
{
	return 0;
}

Packet *
Filter::simple_action(Packet *p_in) {
	
		WritablePacket * p = p_in->uniqueify();
		click_ether *ethh = p->ether_header();
		unsigned short cogtype=0x1234;
		//check it is a cog frame packet or not
		if(ethh->ether_type==CogHeader::COGTYPE)
		{
			
				output(0).push(p_in);
				//	prinff("cogheader\n")
				//	uint8_t* source_address;
				//	struct CogHeader * format = (struct CogHeader *) (p->data()+14);
				//	printf("format \n");
					
				//	source_address=format-> source_eth[0];
				//	printf("here \n");
				//	Controller::getInstance().update_neighbors_ut8(source_address);
				//	printf("end cogheader \n");
		}else{
		//not cog frame packet and it is a user packet
			printf("luanch header \n");
			//extracte the ether header source and distination and make filtering
				uint8_t source_address[6];
				memcpy(source_address, ethh->ether_shost, 6);

				uint8_t dist_address[6];
				memcpy(dist_address, ethh->ether_dhost, 6);

			//check that i am the distination of the packet
				if(Controller::getInstance().is_me(dist_address)){
					printf("is meee \n");
					//check that the source is my neighbor
					if (Controller::getInstance().is_neighbor(source_address)) {
						printf("\n .......................passed..................... \n");
						output(1).push(p_in);
						//else check that the source is primiary user
					}else if(Controller::getInstance().is_PU(source_address)){
						printf("\n PU detecteddddddddddddddddddddddd \n");
						output(2).push(p_in);
					}
				}
	
				printf("\n Didn't pass \n");
	
		}
		
	
	return 0;

}
CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(Filter)
