#pragma once

#ifndef EVENTLOG_H
#define EVENTLOG_H

#include "order.h"
#include <vector>
#include <iostream>
class Eventlog
{
    private: 
        std::vector<Order> log;
        int size; 
    public: 
        Eventlog(); 

        void addOrder(Order new_order); 

        void print() const; 
    
};

#endif
