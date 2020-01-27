
#include "Pricermain.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <any>

Pricermain::Pricermain(std::string inforcefile, std::string scen_folder) {
	loadInforce(inforcefile);
	loadScenario(scen_folder);
	std::cout << "Finished loading." << std::endl;
	pricerMap["ABRP"] = createInstance<PricerABRP>();
	std::cout << "Created pricer." << std::endl;
}

void Pricermain::loadInforce(std::string inforcefile){
	std::cout << "Load inforce file" << std::endl;
	std::ifstream infile1(inforcefile);
	std::string line, data;
	std::vector<std::string> header;
	//Get header line
	std::getline(infile1, line);
	if (*line.rbegin() == '\r')
	    {
	        line.erase(line.length() - 1);
	    }
	std::istringstream iss(line);

	int i = 0;
    while (std::getline(iss,data, ',')) {

    	header.push_back(data);
    	i++;

    }
    iss.clear();
	while (std::getline(infile1, line)){
    	if (*line.rbegin() == '\r')
    	    {
    	        line.erase(line.length() - 1);
    	    }
		std::map<std::string, std::string> rowdata;
	    std::istringstream iss1(line);

	    int j = 0;
	    while (std::getline(iss1,data, ',') ) {

	    	rowdata[header[j]] = data;
	    	j++;
	    }

	    iss1.clear();
	    Policy p;
	    p.issueDate = Date::stringtodate(rowdata["issueDate"]);
	    p.matDate = Date::stringtodate(rowdata["matDate"]);
	    p.birthDate = Date::stringtodate(rowdata["birthDate"]);
	    p.currentDate = Date::stringtodate(rowdata["currentDate"]);


	    p.baseFee = std::stod(rowdata["baseFee"]);
	    p.riderFee = std::stod(rowdata["riderFee"]);
	    p.rollUpRate = std::stod(rowdata["rollUpRate"]);
	    p.gbAmt = std::stod(rowdata["gbAmt"]);
	    p.gmwbBalance = std::stod(rowdata["gmwbBalance"]);
	    p.wbWithdrawalRate = std::stod(rowdata["wbWithdrawalRate"]);
	    p.withdrawal = std::stod(rowdata["withdrawal"]);
	    p.survivorShip = std::stod(rowdata["survivorShip"]);

	    for (int k = 0; k < param.getNumFund(); k++){
	    	std::string fundValuek = "FundValue" + std::to_string(k+1);
	    	std::string fundFeek = "FundFee" + std::to_string(k+1);
	    	p.fundValue.push_back(std::stod(rowdata[fundValuek]));
	    	p.fundFee.push_back(std::stod(rowdata[fundFeek]));
	    }

	    p.gender = rowdata["gender"][0];
	    p.recordID = rowdata["recordID"];
		p.productType = rowdata["productType"];

	    inforce.push_back(p);

	}
	infile1.close();


}

void Pricermain::loadScenario(std::string scen_folder) {
	std::cout << "Load scenario file" << std::endl;
	for (auto const & ir_shock : param.getIRShockList()){
		//Load index return
		std::map<int, std::vector<std::vector<double>>>indexScenario;

		int  i = 0;
		for (auto const index : param.getIndexMap()){
			std::vector<std::vector<double>> scenario;
			std::string indexfile = index + ".csv";
			std::string scenario_file = scen_folder+ir_shock + "/" + indexfile;
			std::ifstream infile(scenario_file);
			std::string line, data;

			while (std::getline(infile, line))
			{
				if (*line.rbegin() == '\r')
				{
					line.erase(line.length() - 1);
				}
			    std::istringstream iss(line);
			    std::getline(iss,line, ','); //Skip the first column
			    std::vector<double> scenarioline;
			    while (std::getline(iss,data, ',')) {
			    	scenarioline.push_back(stod(data));
			    }
			    iss.clear();
			    scenario.push_back(scenarioline);
			}
			infile.close();
			indexScenario[i] = scenario;
			i++;
		}
		//Load forward rate
		std::string fw_file = scen_folder+'/'+ir_shock+"/ForwardCurve.csv";
		std::ifstream infile(fw_file);
		std::string line, data;
		std::getline(infile, line);
		std::getline(infile, line); //Skip the first two lines
		std::vector<double> irRates;
		while (std::getline(infile, line))
		{
			irRates.push_back(stod(line));
		}
		infile.close();

		irIndexScenario[ir_shock] = indexScenario;
		fw[ir_shock]= irRates;
	}

}

void Pricermain::valuation(std::string outputfile){

	std::cout << "Test!!" << std::endl;
    std::ofstream outfile;
    outfile.open (outputfile);
    outfile << "recordID";
    std::vector<std::string> shocknames;

    int i = 0;
    for (auto const & shockname : param.getShockMap()){
    	outfile << ',';
    	outfile << shockname.first ;
    	shocknames.push_back(shockname.first);
    	i++;
    }
    outfile << std::endl;


    int numPol = 0;
    for (auto  const & policy : inforce){
        Pricer*  riderPricer = pricerMap[policy.productType];
        outfile << policy.recordID;
        for (int j = 0; j < shocknames.size(); j++){
        	Shock shocktype;
        	shocktype.shockname = shocknames[j];
        	shocktype.indexshock = param.getShockMap()[shocknames[j]];
        	std::stringstream ss(shocktype.shockname);
        	std::string ir_shock;
        	std::getline(ss, ir_shock, ':');
            //Valuation
            std::cout << "Shock: " << shocktype.shockname << ", valuing record "
            		<< policy.recordID << std::endl;
            PolicyResult res = riderPricer->evaluate(policy, shocktype, fw[ir_shock],
            		irIndexScenario[ir_shock], param.getFemaleMC(), param.getMaleMC());
            outfile << ',';
            outfile  << res.fmv;
            std::cout << "Shock: " << shocktype.shockname << ", Done Valuing"
            		<< policy.recordID << std::endl;
        }
        numPol++;
        outfile << std::endl;
    }
    std::cout << "Done valuation." << std::endl;
    std::cout << "Number of policies: " << numPol << std::endl;



}

void Pricermain::testFunction(){
	std::cout << "This is a test function." << std::endl;
	 //Derived d;
	 //d.fun();
	//Derived d;
	//std::cout << "x = " << d.x << std::endl;
	PricerABRP p;
	std::cout << "dt = " << p.dt << std::endl;
}

Pricermain::~Pricermain() {
	// TODO Auto-generated destructor stub
}

