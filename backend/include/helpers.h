#pragma once

#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <iomanip>
#include <sstream> 


// for the 2018-08-21 first input
bool isValidDate(std::string string_);  // to be implemented 
bool isWeekday(int year, int month, int day);
bool inRange(int year, int month, int day); 

// for the HH:MM::SS second input 
bool isValidTime(std::string string_); // to be implemented 
uint64_t toNanoSeconds(std::string string_); 

std::string timetoString(uint64_t time);


// for ticker input 
bool isValidTicker(std::string string_); // to be implemented


#endif
