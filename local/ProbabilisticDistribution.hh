#ifndef PROBABILISTICDISTRIBUTION_HH
#define PROBABILISTICDISTRIBUTION_HH

class ProbabilisticDistribution { public:

	virtual double getTime() {};
	virtual ProbabilisticDistribution* getInstance(char* args) = 0;	

};

#endif
