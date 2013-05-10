#include <elements/local/ProbabilisticDistribution.hh>
#include <math.h>
#include <stdlib.h> 

using namespace std;

class ExponentialDistribution : public ProbabilisticDistribution { public:

	private:
		double lambda;
		ExponentialDistribution(char* str) {
			lambda = atof(str);
		}

	public:
	
		ExponentialDistribution() {
		}
	

		~ExponentialDistribution() {

		}

		double getTime() {
			double p_x = rand();
			p_x /=RAND_MAX;
			printf("%f\n", -log(1-p_x)/lambda);			
			return -log(1-p_x)/lambda;
		}
		ProbabilisticDistribution* getInstance(char* str){
			return new ExponentialDistribution(str);
		}

};
