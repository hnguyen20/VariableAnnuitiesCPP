
#include <iostream>
#include <ctime>

using namespace std;
#include <stdio.h>
#include <time.h>
#include "Date.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include<string>
#include <sstream>
#include <iomanip>



Date::Date() {
	// TODO Auto-generated constructor stub

}

Date::~Date() {
	// TODO Auto-generated destructor stub
}

std::tm Date::stringtodate(std::string date_string){
    /*
    Convert a string with format dd/mm/YYYY
    */
	std::string data;
	std::tm tmDate;
	std::istringstream ss(date_string);
	std::getline(ss, data, '/');
	tmDate.tm_mday = std::stoi(data);
	std::getline(ss, data, '/');
	tmDate.tm_mon = std::stoi(data);
	std::getline(ss, data, '/');
	tmDate.tm_year = std::stoi(data);
    return tmDate;
}

std::tm Date::stringtodate(const char* date_string){
    /*
    Convert a string with format dd/mm/YYYY
    */
	std::tm tmDate;
	sscanf(date_string,"%2d/%2d/%4d",&tmDate.tm_mday,&tmDate.tm_mon,&tmDate.tm_year);
    return tmDate;
}

int Date::month_between(std::tm dateto, std::tm datefrom){
	int num_months = (dateto.tm_year-datefrom.tm_year)*12 + dateto.tm_mon - datefrom.tm_mon;
	return num_months;
}

double Date::year_between(std::tm dateto, std::tm datefrom){
	std::time_t to = std::mktime(&dateto);
	std::time_t from = std::mktime(&datefrom);
	double days_diff;
	if ( to != (std::time_t)(-1) && from != (std::time_t)(-1) ){
		days_diff = std::difftime(to, from) / (60 * 60 * 24);
	}
	return days_diff/365;
}

std::tm Date::add_months(std::tm date, int months){
	boost::gregorian::date gregDate = boost::gregorian::date_from_tm(date);
	boost::gregorian::month_iterator m_itr(gregDate);
	for (int i = 0; i< months; i++){
		++m_itr;
	}
	return boost::gregorian::to_tm(*m_itr);
}



