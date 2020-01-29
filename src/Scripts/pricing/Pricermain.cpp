
#include "Pricermain.h"
#include <fstream>
#include <iostream>
#include <sstream>

Pricermain::Pricermain(std::string inforcefile, std::string scen_folder) {
	loadInforce(inforcefile);
	loadScenario(scen_folder);
	std::cout << "Finished loading." << std::endl;
	pricerMap["ABRP"] = createInstance<PricerABRP>();
	pricerMap["ABRU"] = createInstance<PricerABRU>();
	pricerMap["ABSU"] = createInstance<PricerABSU>();
	pricerMap["DBAB"] = createInstance<PricerDBAB>();
	pricerMap["DBIB"] = createInstance<PricerDBIB>();
	pricerMap["DBMB"] = createInstance<PricerDBMB>();
	pricerMap["DBRP"] = createInstance<PricerDBRP>();
	pricerMap["DBRU"] = createInstance<PricerDBRU>();
	pricerMap["DBSU"] = createInstance<PricerDBSU>();
	pricerMap["DBWB"] = createInstance<PricerDBWB>();
	pricerMap["IBRP"] = createInstance<PricerIBRP>();
	pricerMap["IBRU"] = createInstance<PricerIBRU>();
	pricerMap["IBSU"] = createInstance<PricerIBSU>();
	pricerMap["MBRP"] = createInstance<PricerMBRP>();
	pricerMap["MBRU"] = createInstance<PricerMBRU>();
	pricerMap["MBSU"] = createInstance<PricerMBSU>();
	pricerMap["WBRP"] = createInstance<PricerWBRP>();
	pricerMap["WBRU"] = createInstance<PricerWBRU>();
	pricerMap["WBSU"] = createInstance<PricerWBSU>();

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
			    std::getline(iss,data, ','); //Skip the first column
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
			if (*line.rbegin() == '\r')
			{
				line.erase(line.length() - 1);
			}
			irRates.push_back(stod(line));
		}
		infile.close();

		irIndexScenario[ir_shock] = indexScenario;
		irFW[ir_shock]= irRates;
	}

}

void Pricermain::valuation(std::string outputfile){

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
            //Valuation
            std::cout << "Shock: " << shocktype.shockname << ", valuing record "
            		<< policy.recordID << std::endl;
            PolicyResult res = riderPricer->evaluate(policy, shocktype);
            outfile << ',';
            outfile  << res.fmv;
            std::cout << "Shock: " << shocktype.shockname << ", Done Valuing"
            		<< policy.recordID << std::endl;
        }
        numPol++;
        outfile << std::endl;
    }
    outfile.close();
    std::cout << "Done valuation." << std::endl;
    std::cout << "Number of policies: " << numPol << std::endl;
}

void Pricermain::valuationScenario(int numRepScenario, std::string outputfolder){

    std::ofstream outfile;
    int numPol = 0;
    for (auto  const & policy : inforce){
		std::cout << "valuing record "
				<< policy.recordID << std::endl;
    	std::string outputfile = outputfolder + policy.recordID + ".csv";
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

        Pricer*  riderPricer = pricerMap[policy.productType];

        for (int sceInd = 0; sceInd < numRepScenario; sceInd++){
        	outfile << policy.recordID;
			for (int j = 0; j < shocknames.size(); j++){
				Shock shocktype;
				shocktype.shockname = shocknames[j];
				shocktype.indexshock = param.getShockMap()[shocknames[j]];
				/*Valuation*/

				PolicyResult res = riderPricer->evaluateScenario(policy, sceInd, shocktype);
				outfile << ',';
				outfile  << res.fmv;


			}
			outfile << std::endl;
        }
        outfile.close();
		std::cout << "Done Valuing" << policy.recordID << std::endl;

        numPol++;
    }
    std::cout << "Done valuation." << std::endl;
    std::cout << "Number of policies: " << numPol << std::endl;
}




Pricermain::~Pricermain() {
	// TODO Auto-generated destructor stub
}

