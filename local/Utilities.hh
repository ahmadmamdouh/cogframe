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
	
	static string convert_String_to_string(String str){
		char* c = str.mutable_c_str();
		return string(c);
	}
	
};

#endif
