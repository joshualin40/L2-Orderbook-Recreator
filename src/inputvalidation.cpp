#include "inputvalidation.h"
#include <ctime>
#include <iostream>

bool isWeekday(int year, int month, int day) {
    std::tm time_in = {0};  // time_in is a struct of type tm 
    time_in.tm_year = year - 1900; // Years since 1900
    time_in.tm_mon = month - 1;    // 0-11
    time_in.tm_mday = day;         // 1-31
    time_in.tm_isdst = -1;         // Let system determine DST

    // mktime updates the tm_wday member (0=Sun, 1=Mon, ..., 6=Sat), part of ctime library that takes a tm struct and determines the weekday
    if (std::mktime(&time_in) == -1) { 
        return false; // Invalid date
    }

    // Weekdays are Monday (1) through Friday (5)
    return (time_in.tm_wday >= 1 && time_in.tm_wday <= 5);
}

bool inRange(int year, int month, int day)
{
    std::tm time_in = {0, 0,0,day, month - 1, year - 1900};
    time_in.tm_isdst = -1; 
    std::tm start_date = {0,0,0, 1, 4, 118}; // 2018-05-01 UTC
    start_date.tm_isdst = -1;

    std::time_t targetTime = std::mktime(&time_in);
    std::time_t startTime = std::mktime(&start_date);

    std::time_t endTime = std::time(nullptr); 

    return (targetTime >= startTime && targetTime <= endTime);
}