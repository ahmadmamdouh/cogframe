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
		.read_mp("CH0", _pu_behavior0)
		.read_mp("CH1", _pu_behavior1)
		.read_mp("CH2", _pu_behavior2)
		.complete() < 0)
      return -1;
		
	Controller controller = Controller::getInstance();
	RouteEntry routeEntry = controller.get_sensed_Channel(controller.getAddress());
	_pu_behavior0 = routeEntry.pu_behavior;
	_pu_behavior1 = routeEntry.pu_behavior1;
	_pu_behavior2 = routeEntry.pu_behavior2;
		
	
	lch.type =  launch_ctrl_hdr::LAUNCH_RES;
	click_chatter("MY IP %s \n",lch.neighbor_ip.unparse().c_str());
	//A node responds with the best channel from three candidates 
	//in terms of PU behaviour(1/(probability of the presence of PU))

	//if (_pu_behavior0 >= _pu_behavior1 && _pu_behavior0 >= _pu_behavior2)
	{
		lch.channel=1;
		lch.pu_behavior = (uint32_t)(_pu_behavior0*100.0);
	}
	//else if (_pu_behavior1 >= _pu_behavior0 && _pu_behavior1 >= _pu_behavior2)
	{
		lch.channel1=6;
		lch.pu_behavior1 = (uint32_t)(_pu_behavior1*100.0);
	}
	//else if (_pu_behavior2 >= _pu_behavior0 && _pu_behavior2 >= _pu_behavior1)
	{
		lch.channel2=11;
		lch.pu_behavior2 = (uint32_t)(_pu_behavior2*100.0);
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

