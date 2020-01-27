
#include "../pricing/Pricermain.h"
#include <iostream>
#include <fstream>

int main() {
	std::string inforcefile, scenariofolder, fmvfile, mapfile, greekfile;
	inforcefile = "./src/Data/inforce.csv";
	//inforcefile = "C:\\Users\\hangn\\eclipse-workspace\\VariableAnnuities\\src\\Data\\inforce_full.csv";
	scenariofolder = "./src/Result/inforcevaluation/RN/";
	// Fair market value and partial account value result files
	fmvfile = "./src/Result/inforcevaluation/fmv_seriatim.csv";
	mapfile = "./src/Result/inforcevaluation/MapFileBase.csv";
	greekfile = "./src/Result/inforcevaluation/Greek.csv";

	Pricermain p(inforcefile,scenariofolder);

	//Output fairmarket values for various shock scenarios
	p.valuation(fmvfile);
	//p.testFunction();

	std::ofstream outfile ("test.txt");

	outfile << "my text here!" << std::endl;

	outfile.close();

	Param param;

	std::cout << param.getMaleMC().p(25,3) << std::endl;


	return 1;

}


