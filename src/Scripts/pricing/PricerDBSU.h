

#ifndef SCRIPTS_PRICING_PRICERDBSU_H_
#define SCRIPTS_PRICING_PRICERDBSU_H_
#include "Pricer.h"

class PricerDBSU: public Pricer {
public:
	PricerDBSU(std::map<std::string, std::map<int, std::vector<std::vector<double>>>> irIndexScen,
				std::map<std::string, std::vector<double>> fwRates): Pricer(irIndexScen,fwRates){}
	void project(Policy& policy, std::map<int, std::vector<std::vector<double>>> &, std::vector<double> &, int scenario_i, int timestep_j);
};

#endif /* SCRIPTS_PRICING_PRICERDBSU_H_ */
