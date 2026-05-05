#pragma once
#ifndef L2snapshot_h
#define L2snapshot_h

#include <map>
#include <iostream>
#include "order.h"
#include "orderbook.h"
#include <sstream>

class Orderbook; // forward declaration

struct L2snapshot
{
    uint64_t time; // in nanoseconds, time since midnight of the day 
    std::map<int64_t, int64_t> bids; // stored as price, quantity. ascending
    std::map<int64_t, int64_t, std::less<int64_t>>  asks; // stored price, quantity 

    void takeSnapshot(Orderbook const& orderbook_); // copies the contents of the Orderbook bids/asks maps into L2snapshot
    void print() const; 
    std::string getTimeStamp() const; 
};

#endif 