#include "eventlog.h"

Eventlog::Eventlog()
{
    size = 0; 
}

void Eventlog::addOrder(Order new_order)
{
    log.push_back(new_order);
    size++; 
}

void Eventlog::print() const
{
     std::cout << std::left
          << std::setw(16) << "TIMESTAMP"
          << std::setw(16) << "SESSION"
          << std::setw(12) << "ACTION"
          << std::setw(8)  << "SIDE"
          << std::setw(16) << "ORDER ID"
          << std::setw(14) << "PRICE"
          << std::setw(10) << "SIZE"
          << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << std::left
              << std::setw(16) << log[i].getTimestamp()
              << std::setw(16) << log[i].getSession() 
              << std::setw(12) << log[i].getAction() 
              << std::setw(8)  << log[i].getSide()
              << std::setw(16) << log[i].getDisplayOrderID()
              << std::setw(14) << log[i].getDisplayPrice()
              << std::setw(10) << log[i].getDisplayQuantity()
              << std::endl;
    }
}