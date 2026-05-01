#pragma once

#ifndef ORDER_H
#define ORDER_H

#include <cstdint> // uint64_t, uint32_t, uint8_t, int64_t, int32_t etc.
#include <string>
#include <sstream> 
#include <ctime>
#include <iomanip>

class Order
{
    private: 
    // we should include timestamp, event, order ID, price, size,
        uint64_t timestamp;    // raw number of nanoseconds since the UNIX epoch
        uint64_t orderid;   
        uint32_t quantity; 
        int64_t price; // The order price where every 1 unit corresponds to 1e-9, i.e. 1/1,000,000,000 or 0.000000001.
        char side; // A (ask), B (Bid), or None
        char action; // Can be Add, Cancel, Modify, cleaR book, Trade, Fill, or None
                    // Trade and Fill actions are
    public:
        Order(uint64_t timestamp_, uint64_t orderid_, uint32_t quantity_, int64_t price_, char side_, char action_); 
        /// string getters (for display)
        std::string getTimestamp() const;  // returns time string in the format of HH:MM:SS.XXX
        std::string getSession() const; // get the session eg. SYS, PREMARKET, MARKET, AFTER HOURS
        std::string getAction() const; // get the type of action eg. add, cancel, modify
        std::string getSide() const; // get the side. ie. Ask, Bid, None
        std::string getDisplayOrderID () const;
        std::string getDisplayPrice() const;
        std::string getDisplayQuantity() const;
        // raw data type getters (for Orderbook calculations)
        char getActionChar() const; // simple getter for the action char 
        char getSideChar() const; 
        uint64_t getOrderID() const;
        int64_t getPrice() const; // returns the original number. ie the price where every 1 unit corresponds to 1e-9
        uint32_t getQuantity() const; 

        // setters
        void setPrice(int64_t new_price); 
        void setQuantity(uint32_t new_quantity);
        
};

#endif