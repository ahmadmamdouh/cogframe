#include <click/config.h>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <elements/local/RFFrontEndWifi.hh>
#include <string>
#include "iwlib.h"
CLICK_DECLS

using namespace std;
/**
*/

RFFrontEndWifi::RFFrontEndWifi()
{
}

RFFrontEndWifi::~RFFrontEndWifi()
{
}

int
RFFrontEndWifi::configure(Vector<String> &conf, ErrorHandler * errh)
{
	if (Args(conf, this, errh)
	       .read_mp("spectrumManager", reinterpret_cast<Element *&>(_spectrum_manager))
	      .complete() < 0)
	      return -1;
	      _spectrum_manager -> registerRFFrontEnd(this);
	return 0;
}


void
RFFrontEndWifi::change_channel(String _if_name, uint8_t channel,String type){
click_chatter("#####RFFRONTEND_CHANNELCHANGE#####");
	printf("Channel_Switching %s %d\n",_if_name.c_str(), channel);
	char* if_name = _if_name.c_str();
	char buffer [3000]; // buffer for system calls.
	int n;
	char name[30];
	int hostID = (type[0]=='R')? 13 : 14;
	n = sprintf(name, "Adhoc_Channel_%d\0", channel);
	printf("sudo ifconfig %s down\n",if_name);
	n = sprintf (buffer, "sudo ifconfig %s down\0",if_name);
	n = system(buffer);
	printf("sudo iwconfig %s mode Ad-Hoc channel %d\n",if_name,channel);
	n = sprintf (buffer, "sudo iwconfig %s mode Ad-Hoc channel %d\0",if_name,channel);
	n = system(buffer);
	printf("sudo ifconfig %s up\n",if_name);
	n = sprintf (buffer, "sudo ifconfig %s up\0",if_name);
	n = system(buffer);
	printf("sudo iwconfig %s essid %s \n",if_name,name);
	n = sprintf (buffer, "sudo iwconfig %s essid %s\0",if_name,name);
	n = system(buffer);
	printf("Network_Creation %s %s\n",_if_name.c_str(), name);
	printf("sudo ifconfig %s 192.168.%d.%d netmask 255.255.255.0\n",if_name,channel,hostID);
	printf("network_ip %s 192.168.%d.%d\n",if_name,channel,hostID);
	n = sprintf (buffer, "sudo ifconfig %s 192.168.%d.%d netmask 255.255.255.0\0",if_name,channel,hostID);
	n = system(buffer);
	printf("Done Switching..\n");

}
void 
RFFrontEndWifi::scan(String _ifname){
	click_chatter("####RFFRONTEND_SCAN####");
	char buffer [3000];	
	int n = sprintf (buffer, "sudo iwlist %s scan",_ifname.c_str());
	n = system(buffer);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RFFrontEndWifi)

