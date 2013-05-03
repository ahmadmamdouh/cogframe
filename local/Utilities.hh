#ifndef Utilities_HH
#define Utilities_HH
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <cstring>

using namespace std;


/*
This class contains general use functions used by different cog-frame modules.
*/
class Utilities { public:
	Utilities(){}
	~Utilities(){}

	// convert uint8_t* mac address to string
	static string convert_uint8_to_string(uint8_t* eth){
		char buffer[100];
		sprintf(buffer, "%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x\0", eth[0], eth[1],
				eth[2], eth[3], eth[4], eth[5]);
		return string(buffer);
	}
	// fill the given array with my addresses and return number of addresses i have
	static void convert_String_to_uint8_t(string stringAddress, uint8_t  source_eth[6])
	{
		char * address=stringAddress.c_str();
		printf("my addressss %s \n" , address);
		int i=0;
		int j=0;
		//convert address from string to uint8_t array 
		for(i=0 ;i<6 ;i++)
		{
		 	char hex[3];
		  	hex[0]=address[j];
		  	hex[1]=address[j+1];
		  	hex[3]="\0";
		  	printf("my part address %s\n", hex);
		  	j=j+3;
		  	
			int d;
			sscanf(hex,"%02x", &d);
			source_eth[i]=(unsigned char)d;
		}
	}
	static string convert_String_to_string(String str){
		char* c = str.mutable_c_str();
		return string(c);
	}
	
	static long getCurrentTime()
	{
		struct timeval start;
		long mtime, seconds, useconds;    
		gettimeofday(&start, NULL);
		seconds  = start.tv_sec; // seconds since epoch
		useconds = start.tv_usec; // microSeconds since epoch
		mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
		return mtime;
	}
	
};

#endif
