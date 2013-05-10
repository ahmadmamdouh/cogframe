#include <elements/local/ProbabilisticDistribution.hh>
#include <math.h>
#include <stdlib.h> 

using namespace std;

class UniformDistribution : public ProbabilisticDistribution { public:

	double a,b;
	private:
	UniformDistribution(char* str) {
		char* abuffer = new char[100];
		char* aptr = abuffer;
		while(*str != ' ') 
			*abuffer++ = *str++;
		str++ , *abuffer = '\0';
		
		a = atof(aptr);
		b = atof(str);	
	}
	public:	
	UniformDistribution() {
	}
	

	~UniformDistribution() {

	}
	UniformDistribution* getInstance(char* args){
		return new UniformDistribution(args);
	}
	
	double getTime() {
			double p_x = a + ((b-a) * rand() / RAND_MAX);
			printf("Time generated: %f\n", p_x);
			
			return p_x;
	}

};
