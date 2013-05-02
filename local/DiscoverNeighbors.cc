#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <elements/local/DiscoverNeighbors.hh>
#include <elements/local/Controller.cc>

using namespace std;
CLICK_DECLS

DiscoverNeighbors::DiscoverNeighbors() {
	// TODO Auto-generated constructor stub
	

}

DiscoverNeighbors::~DiscoverNeighbors() {
	// TODO Auto-generated destructor stub
}


static DiscoverNeighbors& 
DiscoverNeighbors::getInstance(){
	static DiscoverNeighbors instance;
		
		return instance;
}

int
DiscoverNeighbors::configure(Vector<String> &conf, ErrorHandler *errh)
{
  Controller::getInstance().set_discoverneighbors(this); 
 // Controller::getInstance().discover();
	return 0;
}

int
DiscoverNeighbors::initialize(ErrorHandler *)
{
	return 0;
}
void 
DiscoverNeighbors ::discover(){
  	//create new pachet with cogHeader
	WritablePacket *packet = Packet::make((void *)&_lh, sizeof(_lh));
	if (packet == 0 )
		return click_chatter( "cannot make packet!");
	
	struct CogHeader * format = (struct CogHeader *) packet->data();
		
	//format->ether ="00:21:cc:6d:1e:df" ;
	//get my addresses
	uint8_t  my_addresses [10][6] ;
	int my_addresses_count=Controller::getInstance().getAddress_ut8(my_addresses);
	// fill cogHeader
	format->type=CogHeader::Discover;
	memcpy(format->source_eth, my_addresses,sizeof(my_addresses));
	format->number_of_addresses=my_addresses_count;
	
	//make the ether header
	//ether source
	uint8_t  * source_eth;
	source_eth= my_addresses[0];
	
	//print source ether
	char bufferr[100];
	sprintf(bufferr, "%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x\0", source_eth[0], source_eth[1],
				source_eth[2], source_eth[3], source_eth[4], source_eth[5]);
	printf("discover packet making %s\n",bufferr);
	
	//ether distination with broadcast
	uint8_t   dist_eth[6];
	
  		for(int i = 0; i < 6; i++)
				dist_eth[i] = 255;
		char bufferr1[100];
	sprintf(bufferr1, "%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x\0", dist_eth[0], dist_eth[1],
				dist_eth[2], dist_eth[3], dist_eth[4], dist_eth[5]);
	printf("discover packet making %s\n",bufferr1);
	//set the source and dist adresses into ether header
	 struct click_ether ethh;
	 memcpy(ethh.ether_shost, source_eth,6);
	 memcpy(ethh.ether_dhost, dist_eth, 6);
	 
	 //set ether type so that we can identify the packet at filter
	 ethh.ether_type = 0x1234;
	 //Push Ethernet header
	 WritablePacket *q = packet->push(14);
	 memcpy(q->data(), &ethh, 14);
	  q->set_user_anno_u8(1,2);
		printf("pushing discover packet\n");
		// output the discovery packet to output 0
	 output(0).push(q);
}
CLICK_ENDDECLS
ELEMENT_REQUIRES(userlevel int64)
EXPORT_ELEMENT(DiscoverNeighbors)
