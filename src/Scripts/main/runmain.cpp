
#include "../pricing/Pricermain.h"
#include <iostream>
#include <fstream>
#include <chrono>

int main() {
	std::string inforcefile, scenariofolder, fmvfile, mapfile, greekfile,scenarioresultfolder;
	inforcefile = "./src/Data/inforce_small.csv";
	//inforcefile = "C:\\Users\\hangn\\eclipse-workspace\\VariableAnnuities\\src\\Data\\inforce_full.csv";
	scenariofolder = "./src/Result/inforcevaluation/RN/";
	// Fair market value and partial account value result files
	fmvfile = "./src/Result/inforcevaluation/fmv_seriatim.csv";
	mapfile = "./src/Result/inforcevaluation/MapFileBase.csv";
	greekfile = "./src/Result/inforcevaluation/Greek.csv";
	scenarioresultfolder = "./src/Result/inforcevaluation/ScenarioResult/";

	//Output fairmarket values for various shock scenarios
	Pricermain p(inforcefile,scenariofolder);
/*
	std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
	p.valuation(fmvfile);
	std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1).count()/1000000 << "[s]" << std::endl;
*/
	std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
	p.valuationScenario(100, scenarioresultfolder);
	std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
	std::cout << "Scenario Run Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count()/1000000 << "[s]" << std::endl;

	std::ofstream outfile ("test.txt");

	outfile << "my text here!" << std::endl;

	outfile.close();

	Param param;

	std::cout << param.getMaleMC().p(25,3) << std::endl;


	return 1;

}


