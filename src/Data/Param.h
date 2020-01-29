
#ifndef DATA_PARAM_H_
#define DATA_PARAM_H_
#include <string>
#include <map>
#include "../Scripts/utils/Mortality.h"
#include <vector>
#include <set>

class Param {
public:
	static const int NUMINDEX = 5, NUMSCENARIO = 1000, NUMSTEP = 360, NUMYEAR = 30;
	double dt;
	int  numfund;
	std::vector<std::string> indexmap{"LargeCapEquity","SmallCapEquity","IntlEquity","FixedIncome","Money"};
	double cov_return[NUMINDEX][NUMINDEX];


	//Fund map
	std::vector<std::vector<double>> fundmap;

	//Mortality
	Mortality malemc, femalemc;

	//Shock map
	std::map<std::string, std::vector<double>> shockmap;

	//Interest rate shock
	std::set<std::string> irShockList;

	//ProductType
	std::string productType[19] {"ABRP","MBRU","MBRP","DBAB","WBRP","WBSU","WBRU","DBRP","MBSU",
        "IBRP","DBWB","DBIB","DBRU","DBSU","ABSU","IBRU","DBMB","ABRU",
        "IBSU"};;

	public:
		Param();
		virtual ~Param();
		int getNumFund();
		const std::vector<std::vector<double>>& getFundMap();
		Mortality& getMaleMC();
		Mortality& getFemaleMC();
		std::map<std::string, std::vector<double>> getShockMap();
		const std::vector<std::string>& getIndexMap() const;
		std::set<std::string> getIRShockList();
};

#endif /* DATA_PARAM_H_ */
