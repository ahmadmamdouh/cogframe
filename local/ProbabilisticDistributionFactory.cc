#ifndef PROBABILISTIC_DISTRIBUTION_FACTORY
#define PROBABILISTIC_DISTRIBUTION_FACTORY
#include <map>
#include <iostream>
#include <elements/local/ProbabilisticDistribution.hh>
#include <stdio.h>
using namespace std;
class ProbabilisticDistributionFactory {

	private:
		ProbabilisticDistributionFactory() {
		}
		
		~ProbabilisticDistributionFactory() {
		}
		void operator=(ProbabilisticDistributionFactory const&); // Don't implement
		map <string, ProbabilisticDistribution*> probsDistributions;
	public:			
		static ProbabilisticDistributionFactory& getInstance(){
			static ProbabilisticDistributionFactory instance;
			return instance;
		}
		
		ProbabilisticDistribution* getDistribution(string type, char* args){
			ProbabilisticDistribution* p;
			printf("%s %d\n",type.c_str(),probsDistributions.count(type));
			p = probsDistributions[type]->getInstance(args);			  
			return p;
		}
		
		void addDistribution(string type, ProbabilisticDistribution* pd){
			probsDistributions[type] = pd;
		}
};
#endif;
