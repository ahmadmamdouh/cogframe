#include <click/config.h>
#include <elements/local/launch.hh>
#include <elements/local/launchctrlresponder.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <elements/local/Controller.cc>
CLICK_DECLS


LaunchCtrlResponder::LaunchCtrlResponder()
{
}

LaunchCtrlResponder::~LaunchCtrlResponder()
{
}

int
LaunchCtrlResponder::configure(Vector<String> &conf, ErrorHandler * errh)
{

 	if (Args(conf, this, errh)
		.read_mp("ETH", _my_eth)
		.read_mp("IP", lch.neighbor_ip)
		.read_mp("LONG", lch.my_long)
    		.read_mp("LAT", lch.my_lat)
		.read_mp("TSWTCH", lch.switching_time)
		.complete() < 0)
      return -1;
		
	Controller controller = Controller::getInstance();
	map<int, struct Channel>  *channels = controller.get_sensed_Channel();
	
	lch.type =  launch_ctrl_hdr::LAUNCH_RES;
	click_chatter("MY IP %s \n",lch.neighbor_ip.unparse().c_str());
	
	//A node responds with the best channel from three candidates 
	//in terms of PU behaviour(1/(probability of the presence of PU))
	for(map<int,struct Channel>::iterator it = (*channels).begin(); it !=(*channels).end(); it++) {
		struct Channel c = it->second;
		lch.channels_id[lch.channels_size] = c.id;
		lch.channels_pu_prob[lch.channels_size] = c.pu_prob;
		lch.channels_size++;
	}

	return 0;
}




Packet *
LaunchCtrlResponder::simple_action(Packet *p_in)
{
 	 WritablePacket *p = p_in->uniqueify();

  if (!p)
    return 0;

  click_ether *ethh = p->ether_header();
  uint8_t source_address[6];
  memcpy(source_address, ethh->ether_shost, 6);
  memcpy(ethh->ether_dhost, source_address, 6);
  memcpy(ethh->ether_shost, _my_eth.data(), 6);

  memcpy(p->data()+14, &lch, sizeof(lch));

  return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LaunchCtrlResponder)

