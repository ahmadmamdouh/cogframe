#include <click/config.h>
#include <elements/local/CogHeaderParser.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>

CLICK_DECLS

CogHeaderParser::CogHeaderParser()
{
}

CogHeaderParser::~CogHeaderParser()
{
}

int
CogHeaderParser::configure(Vector<String> &conf, ErrorHandler *errh)
{
  
  return 0;
}


Packet *
CogHeaderParser::simple_action(Packet *p)
{
struct CogHeader * ch = (struct CogHeader *) (p->data()+14);
		int type=ch->type;
		printf("cog tyyyyyyyyyyyyyyyyypeeeeeeeeee  %d\n",type);
		switch (type){
			case CogHeader::Discover:
				{
					printf("discoveeeeeeeeeeeer type \n");
					int numAddresses = ch->number_of_addresses;
					printf("number of addresses %d \n",numAddresses);
					for (int i=0 ; i < numAddresses ;i++)
					{
						printf("looooooooooooping \n");
						uint8_t*source_address=ch-> source_eth[i];
						printf("here \n");
						Controller::getInstance().update_neighbors_ut8(source_address);
					}
				}
			break;
			case CogHeader::EndExperiment:
				output(0).push(p);
			break;
  	}
}


CLICK_ENDDECLS
EXPORT_ELEMENT(CogHeaderParser)
