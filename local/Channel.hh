#ifndef CHANNEL_HH
#define CHANNEL_HH


struct Channel { 

	uint8_t id;
	float pu_prob;
	bool pu_active;
	
	Channel() {}
	~Channel() {}
	
	Channel(uint8_t _id, float _prob, bool _pu_active) {
	  id = _id;
	  pu_prob = _prob;
	  pu_active = _pu_active;
	}
	
};

#endif
