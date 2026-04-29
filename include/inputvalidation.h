#pragma once

#ifndef VALIDATETIME_H
#define VALIDATETIME_H

#include <string>

bool isValidDate(std::string string_); 
bool isWeekday(int year, int month, int day);
bool inRange(int year, int month, int day); 

bool isValidTicker(std::string string_);

#endif
