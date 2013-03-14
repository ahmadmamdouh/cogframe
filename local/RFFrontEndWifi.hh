#ifndef RFFRONTENDWIFI_hh
#define RFFRONTENDWIFI_hh
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "iwlib.h"
#include <unistd.h>
class RFFrontEndWifi{ public:

RFFrontEndWifi() {
}

~RFFrontEndWifi() {
}

void change_channel(String _if_name, uint8_t channel,String type) {
	click_chatter("#####RFFRONTEND_CHANNELCHANGE#####");
	char* if_name = _if_name.c_str();
	char buffer [3000]; // buffer for system calls.
	int n;
	char name[30];
	int hostID = (type[0]=='R')? 1 : 2;
	printf(":(((((((((((((((((((((((((((((((((((((((((");
	n = sprintf(name, "Adhoc_Channel_%d\0", channel);
	int itr = 10;
	//while(itr-->0) 
	{
	printf("sudo ifconfig %s down\n",if_name);
	n = sprintf (buffer, "sudo ifconfig %s down\0",if_name);
	n = system(buffer);
	printf("%d\n", n);
//	sleep(3);
	printf("sudo iwconfig %s mode Ad-Hoc channel %d\n",if_name,channel);
	n = sprintf (buffer, "sudo iwconfig %s mode Ad-Hoc channel %d\0",if_name,channel);
	n = system(buffer);
	printf("%d\n", n);
//	sleep(3);
	printf("sudo ifconfig %s up\n",if_name);
	n = sprintf (buffer, "sudo ifconfig %s up\0",if_name);
	n = system(buffer);
	printf("%d\n", n);
//	sleep(3);
	printf("sudo iwconfig %s essid %s \n",if_name,name);
	n = sprintf (buffer, "sudo iwconfig %s essid %s\0",if_name,name);
	n = system(buffer);
	printf("%d\n", n);
//	sleep(3);
	hostID = 22;
	printf("sudo ifconfig %s 192.168.%d.%d netmask 255.255.255.0\n",if_name,channel,hostID);
	n = sprintf (buffer, "sudo ifconfig %s 192.168.%d.%d netmask 255.255.255.0\0",if_name,channel,hostID);
	n = system(buffer);
	printf("%d\n", n);
//	sleep(3);
	}
	printf("Done Switching..\n");
}

void scan(String _ifname){
	click_chatter("####RFFRONTEND_SCAN####");
	char buffer [3000];	
	int n = sprintf (buffer, "sudo iwlist %s scan",_ifname.c_str());
	n = system(buffer);
}

};
#endif
