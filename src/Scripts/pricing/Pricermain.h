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
#include "PricerABRU.h"
#include "PricerABSU.h"
#include "PricerDBAB.h"
#include "PricerDBIB.h"
#include "PricerDBMB.h"
#include "PricerDBRP.h"
#include "PricerDBRU.h"
#include "PricerDBSU.h"
#include "PricerDBWB.h"
#include "PricerIBRP.h"
#include "PricerIBRU.h"
#include "PricerIBSU.h"
#include "PricerMBRP.h"
#include "PricerMBRU.h"
#include "PricerMBSU.h"
#include "PricerWBRP.h"
#include "PricerWBRU.h"
#include "PricerWBSU.h"

class Pricermain {
	std::map<std::string, std::map<int, std::vector<std::vector<double>>>> irIndexScenario;
	std::map<std::string, std::vector<double>> irFW;
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
	void valuationScenario(int, std::string);
	template<typename T> Pricer* createInstance () { return new T(irIndexScenario, irFW);}
	void testFunction();



};

#endif /* SCRIPTS_PRICING_PRICERMAIN_H_ */
