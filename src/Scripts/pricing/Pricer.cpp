
#include "Pricer.h"
#include <iomanip>




Pricer::Pricer(std::map<std::string, std::map<int, std::vector<std::vector<double>>>> irIndexScen,
		std::map<std::string, std::vector<double>> fwRates) {
	irIndexScenario = irIndexScen;
	irFW = fwRates;

	dt = 1.0 * Param::NUMYEAR/Param::NUMSTEP;

	for (int i = 0; i < Param::NUMSCENARIO; i++){
		std::vector<double> row;
		for (int j = 0; j < Param::NUMSTEP; j++){
			row.push_back(0.0);
		}
		AV.push_back(row);
		DA.push_back(row);
		LA.push_back(row);
		RC.push_back(row);
	}
	for (int i = 0; i < Param::NUMSTEP + 1; i++){
		df.push_back(0.0);
		q.push_back(0.0);
		s.push_back(0.0);
	}

}

PolicyResult Pricer::evaluate(Policy policy, Shock shocktype) {
	/*Evaluate for a policy, given a shock value for each fund, forward curve, mortality,
    equity composition of the funds.
    Shock is a map of fund num : fund shock
    */


	PolicyResult res;
    std::vector<double> shock = shocktype.indexshock;
    std::stringstream ss(shocktype.shockname);
	std::string ir_shock;
	std::getline(ss, ir_shock, ':');

	for (int i = 0; i < Param::NUMINDEX; i++) {
		for (int k = 0; k < param.getNumFund(); k++) {
			res.av[i] = res.av[i] + policy.fundValue[k] * (1 + shock[i])
					* param.getFundMap()[k][i];

		}
	}



	//Time to maturity
	int T = Date::month_between(policy.matDate, policy.currentDate);


	if (T < 0){
		return res;
	}
	double age = Date::year_between(policy.currentDate, policy.birthDate);

	df[0] = 1.0; //Discount factor
	q[0] = 0.0; //Death rate
	s[0] = 1.0; //Survival rate


	for (int j = 1; j < Param::NUMSTEP + 1; j++){
		df[j] = df[j-1] * std::exp(-irFW[ir_shock][j-1] * dt);

		if (std::tolower(policy.gender) == 'm'){
			q[j] = param.getMaleMC().q(age + (j-1) * dt, dt);
			s[j] = param.getMaleMC().p(age, j * dt);
		}
		else {
			q[j] = param.getFemaleMC().q(age + (j-1) * dt, dt);
			s[j] = param.getFemaleMC().p(age, j * dt);
		}
	}


	for (int sceInd = 0; sceInd < Param::NUMSCENARIO; sceInd++){
		Policy pol = policy;
		for (int k = 0; k < param.getNumFund(); k++) {
			double fundval_k = 0.0;
			for (int i = 0; i < Param::NUMINDEX; i++){
				fundval_k = fundval_k + pol.fundValue[k] * (1 + shock[i])
						* param.getFundMap()[k][i];
			}
			pol.fundValue[k] = fundval_k;
		}

		for ( int timeInd = 0; timeInd < Param::NUMSTEP; timeInd++){
			project(pol, irIndexScenario[ir_shock], irFW[ir_shock], sceInd, timeInd);
		}
	}



	double pvRC = 0.0, pvDA = 0.0, pvLA = 0.0; //Risk charge, death benefit, living benefit


	for (int j = 1;  j < Param::NUMSTEP +1; j++){
		double rc = 0.0, da = 0.0, la = 0.0;
		for (int i = 0; i< Param::NUMSCENARIO; i++) {
			rc = rc + RC[i][j-1];
			da = da + DA[i][j-1];
			la = la + LA[i][j-1];
		}

		pvRC = pvRC + rc/Param::NUMSCENARIO * df[j] * s[j];
		pvDA = pvDA + da/Param::NUMSCENARIO * df[j] * s[j-1] * q[j];
		pvLA = pvLA + la/Param::NUMSCENARIO * df[j] * s[j];

	}
	res.riskCharge = pvRC * policy.survivorShip;
	res.gmdb = pvDA * policy.survivorShip;
	res.gmlb = pvLA * policy.survivorShip;
	res.guaranteedbenefit = (pvDA+pvLA) * policy.survivorShip;

	//Fair market value
	res.fmv = res.guaranteedbenefit - res.riskCharge;
	return res;

}

PolicyResult Pricer::evaluateScenario(Policy policy, int sceInd, Shock shocktype) {
	/*Evaluate for a policy, given a shock value for each fund, forward curve, mortality,
    equity composition of the funds.
    Shock is a map of fund num : fund shock
    */


	PolicyResult res;
    std::vector<double> shock = shocktype.indexshock;
    std::stringstream ss(shocktype.shockname);
	std::string ir_shock;
	std::getline(ss, ir_shock, ':');

	for (int i = 0; i < Param::NUMINDEX; i++) {
		for (int k = 0; k < param.getNumFund(); k++) {
			res.av[i] = res.av[i] + policy.fundValue[k] * (1 + shock[i])
					* param.getFundMap()[k][i];

		}
	}



	//Time to maturity
	int T = Date::month_between(policy.matDate, policy.currentDate);


	if (T < 0){
		return res;
	}
	double age = Date::year_between(policy.currentDate, policy.birthDate);

	df[0] = 1.0; //Discount factor
	q[0] = 0.0; //Death rate
	s[0] = 1.0; //Survival rate


	for (int j = 1; j < Param::NUMSTEP + 1; j++){
		df[j] = df[j-1] * std::exp(-irFW[ir_shock][j-1] * dt);

		if (std::tolower(policy.gender) == 'm'){
			q[j] = param.getMaleMC().q(age + (j-1) * dt, dt);
			s[j] = param.getMaleMC().p(age, j * dt);
		}
		else {
			q[j] = param.getFemaleMC().q(age + (j-1) * dt, dt);
			s[j] = param.getFemaleMC().p(age, j * dt);
		}
	}


	Policy pol = policy;
	for (int k = 0; k < param.getNumFund(); k++) {
		double fundval_k = 0.0;
		for (int i = 0; i < Param::NUMINDEX; i++){
			fundval_k = fundval_k + pol.fundValue[k] * (1 + shock[i])
					* param.getFundMap()[k][i];
		}
		pol.fundValue[k] = fundval_k;
	}

	for ( int timeInd = 0; timeInd < Param::NUMSTEP; timeInd++){
		project(pol, irIndexScenario[ir_shock], irFW[ir_shock], sceInd, timeInd);
	}




	double pvRC = 0.0, pvDA = 0.0, pvLA = 0.0; //Risk charge, death benefit, living benefit


	for (int j = 1;  j < Param::NUMSTEP +1; j++){
		double rc = 0.0, da = 0.0, la = 0.0;

		rc = rc + RC[sceInd][j-1];
		da = da + DA[sceInd][j-1];
		la = la + LA[sceInd][j-1];



		pvRC = pvRC + rc * df[j] * s[j];
		pvDA = pvDA + da * df[j] * s[j-1] * q[j];
		pvLA = pvLA + la * df[j] * s[j];




	}
	res.riskCharge = pvRC * policy.survivorShip;
	res.gmdb = pvDA * policy.survivorShip;
	res.gmlb = pvLA * policy.survivorShip;
	res.guaranteedbenefit = (pvDA+pvLA) * policy.survivorShip;
	//Fair market value
	res.fmv = res.guaranteedbenefit - res.riskCharge;
	return res;

}



double Pricer::getfundScenario(int fundnum,
		 std::map<int, std::vector<std::vector<double>>> &indexScenario,
		int sceInd, int timeInd) {
    double fundreturn = 0;
    for (int i = 0; i < Param::NUMINDEX; i++){

        fundreturn += param.getFundMap()[fundnum][i]*indexScenario[i][sceInd][timeInd];
    }

    return fundreturn;
}

double Pricer::getAnnuityFactor(Policy policy, int timeInd, std::vector<double> &fw){
	double aT = 0.0;
	double age = Date::year_between(policy.currentDate, policy.birthDate);
	double dP = 1.0;
	int nY = 0;
	while(dP > 1e-4) {
		double dFR = 0.0;
		if(timeInd + nY * 12 < Param::NUMSTEP) {
			dFR = fw[timeInd  + nY * 12];
		}
		else {
			dFR = fw[Param::NUMSTEP-1];
		}
		if (std::tolower(policy.gender) == 'f') {
			dP = param.getFemaleMC().p(age, nY);
		}
		else {
			dP = param.getMaleMC().p(age, nY);
		}
		aT += dP * std::exp(-dFR * nY);
		nY += 1;
	}
	return aT;
}

double Pricer::getAnnuityFactor_r(Policy policy, double r) {
	double aT = 0.0;
	double age = Date::year_between(policy.currentDate, policy.birthDate);
	double dP = 1.0;
	int nY = 0;
	while(dP > 1e-4) {
		if (std::tolower(policy.gender) == 'f') {
			dP = param.getFemaleMC().p(age, nY);
		}
		else {
			dP = param.getMaleMC().p(age, nY);
		}
		aT += dP * std::exp(-r * nY);
		nY += 1;
	}
	return aT;
}
