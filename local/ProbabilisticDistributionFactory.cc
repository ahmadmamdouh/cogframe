#include <elements/local/ExponentialDistribution.cc>
#include <elements/local/UniformDistribution.cc>
#include <elements/local/ProbabilisticDistribution.hh>

#define EXP "exp"
#define UNIFORM "uniform"


class ProbabilisticDistributionFactory {
	public:
		
		ProbabilisticDistributionFactory() {
		}
		
		~ProbabilisticDistributionFactory() {
		}
		
		ProbabilisticDistribution* getDistribution(String type, char* args){
			ProbabilisticDistribution* p;
			
			if (type.equals(EXP, strlen(EXP)))
				p = new ExponentialDistribution(args);
				
			else if (type.equals(UNIFORM, strlen(UNIFORM)))
			  p = new UniformDistribution(args);
			  
			return p;
		}
};
