#include "helpers.h"
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

uint64_t toNanoSeconds(std::string string_)
{
    uint64_t hours = stoull(string_.substr(0,2));
    uint64_t minutes = stoull(string_.substr(3,2));
    uint64_t seconds = stoull(string_.substr(6,2));
    return hours * 3.6e+12 + minutes * 6e+10 + seconds * 1e+9;
}

std::string timetoString(uint64_t time)
{
    long long hours   = time / 3.6e+12;
    long long minutes = (time % (long long)3.6e+12) / 6e+10;
    long long seconds = (time % (long long)6e+10)   / 1e+9;

    std::ostringstream os;
    os << std::setfill('0') << "T"
       << std::setw(2) << hours   << ":"
       << std::setw(2) << minutes << ":"
       << std::setw(2) << seconds;
    return os.str();
}