#ifndef SCRIPTS_PRICING_PRICER_H_
#define SCRIPTS_PRICING_PRICER_H_

#include "../../Data/Param.h"
#include "../../Scripts/utils/Date.h"
#include <time.h>
#include <iostream>
#include <math.h>

struct Policy {
	std::tm issueDate, matDate, birthDate, currentDate;
	double baseFee, riderFee, rollUpRate;
	double gbAmt, gmwbBalance, wbWithdrawalRate,withdrawal,survivorShip;
	std::vector<double> fundValue, fundFee;
	char gender;
	std::string recordID, productType;
};

struct PolicyResult {
    double riskCharge = 0, gmdb = 0, gmlb = 0, guaranteedbenefit = 0, fmv = 0;
    double av[Param::NUMINDEX] = {0};

};


struct Shock {
    /*
    Example: shockname = base:2_D, indexshockk = [0,-0.01,0,0,0] which means
    index 2 goes down by 10 basic points
    */
	std::string shockname;
	std::vector<double> indexshock;
};

class Pricer {
public:
    //Account value, death benefit, living benefit, risk charge, size  numscenario x numstep
	std::vector<std::vector<double>> AV, DA, LA, RC;
	//Discount factor, mortality rate, survivor rate, size numstep+1
	std::vector<double> df, q, s;

	//Time step
	double dt;
	Param param;

public:
	Pricer();
	virtual ~Pricer(){};
	PolicyResult evaluate(Policy, Shock &, std::vector<double> &, std::map<int, std::vector<std::vector<double>>> &,  Mortality &,  Mortality &) ;
	PolicyResult evaluateScenario(Policy, int, Shock &, std::vector<double> &, std::map<int, std::vector<std::vector<double>>> &,  Mortality &,  Mortality &) ;
	virtual void project(Policy&,  std::map<int, std::vector<std::vector<double>>> &, int , int ) = 0;
	double getfundScenario(int,   std::map<int, std::vector<std::vector<double>>> &indexScenario, int, int)  ;
};

#endif /* SCRIPTS_PRICING_PRICER_H_ */
