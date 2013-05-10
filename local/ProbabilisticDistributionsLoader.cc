#ifndef PROBABILISTIC_DISTRIBUTIONS_LOADER
#define PROBABILISTIC_DISTRIBUTIONS_LOADER
#define EXP "exp"
#define UNIFORM "uniform"
#include <elements/local/ProbabilisticDistributionFactory.cc>
#include <elements/local/ProbabilisticDistribution.hh>
#include <elements/local/UniformDistribution.cc>
#include <elements/local/ExponentialDistribution.cc>
class ProbabilisticDistributionsLoader{
	public:
		ProbabilisticDistributionsLoader(){
	
		}
		~ProbabilisticDistributionsLoader(){
	
		}
	void addDistributions(){
		ProbabilisticDistributionFactory::getInstance().addDistribution("uniform",new UniformDistribution());
		ProbabilisticDistributionFactory::getInstance().addDistribution(EXP,new ExponentialDistribution());
	}

};
#endif
