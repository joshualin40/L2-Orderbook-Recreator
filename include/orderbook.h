#pragma once

#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"
#include <map> 
#include <memory>
#include <string>
#include <iostream>
#include <sstream>

// start with implementation of OrderBook class with just 
class Orderbook
{ 
    private:
        std::map<int64_t, std::deque<std::unique_ptr<Order>>, std::greater<int64_t>> bids;  // bids, descending. [key, value] -> price, queue of orders
        std::map<int64_t, std::deque<std::unique_ptr<Order>>, std::less<int64_t>>    asks;  // asks, ascending.  [key, value] -> price, queue of orders
        // orders at the same price level are managed in a FIFO way. a deque (double ended vector) stores the
        int lowestask; 
        int highestbid; 


        std::unordered_map<uint64_t, Order> orders; // keep track of all orders here. [key, value] -> orderid, Order

        void insertOrder(const Order& order);   // action = 'A'. an "add" action
        void cancelOrder(const Order& order);   // action = 'C' a "cancel" action
        void modifyOrder(const Order& order);   // action = 'M' a "modify" action
        void clearBook();                       // action = 'R' a "clear action"

    public:
        Orderbook(); 

                                                
        void processEvent(const Order& new_order);  // routes to correct operation based on action

        std::string getDisplaySpread() const; // returns string of the spread 

        void print() const;                     // displays L2 snapshot

  
};

#endif


