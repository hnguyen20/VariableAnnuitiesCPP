#include "Param.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Param::Param() {

	dt = 1.0 * NUMYEAR / NUMSTEP;

	cov_return[0][0] = 0.1514;
	cov_return[1][1] = 0.2052;
	cov_return[2][2] = 0.1705;
	cov_return[3][3] = 0.0429;
	cov_return[4][4] = 0.0050;
	cov_return[0][1] = 0.7619;
	cov_return[0][2] = 0.5571;
	cov_return[0][3] = 0.2369;
	cov_return[0][4] = -0.0383;
	cov_return[1][2] = 0.4433;
	cov_return[1][3] = 0.1303;
	cov_return[1][4] = -0.0334;
	cov_return[2][3] = 0.1505;
	cov_return[2][4] = -0.0347;
	cov_return[3][4] = 0.0360;



	//Construct fund map

	std::ifstream infile1("./src/Data/FundMap.csv");
	std::string line, data;

	std::getline(infile1, line); //Skip header line
	while (std::getline(infile1, line))
	{
		if (*line.rbegin() == '\r')
		{
			line.erase(line.length() - 1);
		}
		std::vector<double> rowdata;
	    std::istringstream iss(line);
	    std::getline(iss,data, ',');
	    while (std::getline(iss,data, ',')) {
	    	rowdata.push_back(stod(data));
	    }
	    iss.clear();
	    fundmap.push_back(rowdata);
	}
	infile1.close();
	numfund = fundmap.size();


	//Construct mortality
	malemc.Init("./src/Data/1996iammale.csv");
	femalemc.Init("./src/Data/1996iamfemale.csv");


	//Construct shock map

	shockmap["base:base"] = std::vector<double>{0.,0.,0.,0.,0.};




	/* Uncomment this part for Full shock map */
	std::ifstream infile2("./src/Data/ShockMap.csv");
	std::string s1, s2, shockname;


	std::getline(infile2, line); // Skip 1st line.

	while (std::getline(infile2	, line))
	{
		std::vector<double> indexshock;
		if (*line.rbegin() == '\r')
		{
			line.erase(line.length() - 1);
		}
		std::istringstream iss(line);
	    std::getline(iss,s1, ',');
		int index = 0;
		while (std::getline(iss,data, ',')){
			if (index < NUMINDEX) {
				indexshock.push_back(stod(data));
			}
			else {
				s2 = data;
			}
			index++;
		}

		shockname = s1+":"+s2;
		shockmap[shockname] = indexshock;
		//std::cout << "Shock name: " << shockname << "is: "<< shockmap[shockname][2] << std::endl;
	}
	infile2.close();

	//Construct ir shock list
	for (auto const & element: shockmap) {
		std::stringstream shockname(element.first);
		std::string irname;
		std::getline(shockname, irname, ':');
		irShockList.insert(irname);
	}


}

int Param::getNumFund(){
	return numfund;
}

const std::vector<std::vector<double>>& Param::getFundMap(){
	return fundmap;
}
 Mortality& Param::getMaleMC(){
	return malemc;
}

 Mortality& Param::getFemaleMC(){
	return femalemc;
}

std::map<std::string, std::vector<double>> Param::getShockMap(){
	return shockmap;
}


const std::vector<std::string> & Param::getIndexMap() const{
	return indexmap;
}

std::set<std::string> Param::getIRShockList(){
	return irShockList;
}
Param::~Param() {
	// TODO Auto-generated destructor stub
}

