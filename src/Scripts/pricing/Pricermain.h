/*
 * Pricermain.h
 *
 *  Created on: 25 Jan 2020
 *      Author: hangn
 */

#ifndef SCRIPTS_PRICING_PRICERMAIN_H_
#define SCRIPTS_PRICING_PRICERMAIN_H_
#include<string>
#include "Pricer.h"
#include "PricerABRP.h"
#include <any>

class Pricermain {
	std::map<std::string, std::map<int, std::vector<std::vector<double>>>> irIndexScenario;
	std::map<std::string, std::vector<double>> fw;
	std::vector<Policy> inforce;
	Param param;
	typedef std::map<std::string, Pricer*> map_type;
	map_type pricerMap;


public:
	Pricermain(std::string,std::string);
	virtual ~Pricermain();
	void loadInforce(std::string);
	void loadScenario(std::string);
	void valuation(std::string);
	template<typename T> Pricer* createInstance() { return new T();}
	void testFunction();



};

#endif /* SCRIPTS_PRICING_PRICERMAIN_H_ */
