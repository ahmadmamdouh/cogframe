#include <elements/local/ProbabilisticDistribution.hh>
#include <math.h>
#include <stdlib.h> 

using namespace std;

class ExponentialDistribution : public ProbabilisticDistribution { public:

	double lambda;
	
	ExponentialDistribution() {
	}
	
	ExponentialDistribution(char* str) {
		lambda = atof(str);
	}

	~ExponentialDistribution() {

	}

	double getTime() {
			double p_x = rand() * lambda / RAND_MAX;
			printf("Time generated: %f, log: %f\n", p_x, log(p_x));
			
			return (log(lambda) - log(p_x)) / lambda;
	}

};
