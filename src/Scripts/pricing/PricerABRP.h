

#ifndef SCRIPTS_PRICING_PRICERABRP_H_
#define SCRIPTS_PRICING_PRICERABRP_H_
#include "Pricer.h"

class PricerABRP: public Pricer {
public:
	void project(Policy& policy, std::map<int, std::vector<std::vector<double>>> &indexScenario, int scenario_i, int timestep_j);
};

#endif /* SCRIPTS_PRICING_PRICERABRP_H_ */
