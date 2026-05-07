#include "orderbook.h"
#include <databento/record.hpp>
// private functions for processing each order action, ie. add, cancel, modify, clear
namespace db = databento;

void Orderbook::insertOrder(const Order& order) { // add. 
    // in insertOrder, guard against invalid prices
    if (order.getPrice() == INT64_MAX || 
    order.getPrice() <= 0 ||
    order.getQuantity() == 0) return;
    // check if it buy/sell side first. 
    if (order.getSideChar() == 'B') {
        // push order to back of deque at this price level (FIFO)
        bids[order.getPrice()].push_back(std::make_unique<Order>(order));
    } 
    else if (order.getSideChar() == 'A') {
        asks[order.getPrice()].push_back(std::make_unique<Order>(order));
    }
    else if (order.getSideChar() == 'N')
    {
        return; 
    }
    orders.insert({order.getOrderID(), order});// this is only for the add action. cancel and modify don't require a new key,value in the map
}
void Orderbook::cancelOrder(const Order& order)
{
    if (orders.count(order.getOrderID()) == 0) return;  // order not found

    const Order& existing = orders.at(order.getOrderID()); // look up the existing order from the orders map 
    if (existing.getSideChar() == 'B') { // bids
        // iterate through the deque at the price level of order
        // when you find the pointer that matches the orderID, delete it
        // the deque at the price level of order is 
        for (auto it = bids[existing.getPrice()].begin(); it != bids[existing.getPrice()].end(); it++) // it is a iterator pointer
        {
            if ((*it)->getOrderID() == existing.getOrderID())
            {
                bids[existing.getPrice()].erase(it); 
                break;
            }
        }
        // if after the deque is empty, delete it 
        auto it = bids.find(existing.getPrice());
        if (it != bids.end() && it->second.empty())
            bids.erase(it);

    } 
    else if (existing.getSideChar() == 'A') { // asks
       for (auto it = asks[existing.getPrice()].begin(); it != asks[existing.getPrice()].end(); it++) // it is a iterator pointer
        {
            if ((*it)->getOrderID() == existing.getOrderID())
            {
                asks[existing.getPrice()].erase(it); 
                break;
            }
        }
        auto it = asks.find(existing.getPrice());
        if (it != asks.end() && it->second.empty())
            asks.erase(it);
    }
    else if (existing.getSideChar() == 'N')
    {
        // do nothing
    }

    // get rid of the order from ordermap
    orders.erase(order.getOrderID()); 
}
void Orderbook::modifyOrder(const Order& order){ // Modify — Change an order's price and/or size 
    if (orders.count(order.getOrderID()) == 0) return;  // order not found
    // validate new price 
     if (orders.count(order.getOrderID()) == 0) return;
    if (order.getPrice() == INT64_MAX || order.getPrice() <= 0) return; 
    Order& existing = orders.at(order.getOrderID()); 
    auto orderID = existing.getOrderID(); 
   // first check if the quantity changed. this is easier as it doesn't require moving the existing order between deques
   if (existing.getQuantity() != order.getQuantity())
    {
        // change the quantity
        existing.setQuantity(order.getQuantity());  
        // also need to change the order quantity within the deque 
        if (existing.getSideChar() == 'A') // asks
        {
            for (auto it = asks[existing.getPrice()].begin(); it != asks[existing.getPrice()].end(); it++)
            {
                if ((*it)->getOrderID() == orderID)
                {
                    (*it)->setQuantity(order.getQuantity()); 
                    break;
                }
            }
        }
        else
        {
            for (auto it = bids[existing.getPrice()].begin(); it != bids[existing.getPrice()].end(); it++)
            {
                if ((*it)->getOrderID() == orderID)
                {
                    (*it)->setQuantity(order.getQuantity()); 
                    break;
                }
            }
        }

    }
    // check if the price changed. if it has we must move it to a new deque in the corresponding map
    if (existing.getPrice() != order.getPrice())
    {
        auto currentprice = existing.getPrice();
        auto newprice = order.getPrice(); 
        if (existing.getSideChar() == 'B') //bid
        {
            // erase from current deque
            for (auto it = bids[currentprice].begin(); it != bids[currentprice].end(); it++)
            {
                if ((*it)->getOrderID() == orderID) 
                {
                    bids[currentprice].erase(it);
                    break;
                }
            }
            if (bids[currentprice].empty()) // if the deque is empty after delete
                bids.erase(currentprice);
            // add to new deque
            existing.setPrice(newprice);
            bids[newprice].push_back(std::make_unique<Order>(existing));
        }   
        else
        {
            for (auto it = asks[currentprice].begin(); it != asks[currentprice].end(); it++)
            {
                if ((*it)->getOrderID() == orderID) 
                {
                    asks[currentprice].erase(it);
                    break;
                }
            }
            if (asks[currentprice].empty()) // if the deque is empty after delete
                asks.erase(currentprice);
            // add to new deque
            existing.setPrice(newprice);
            asks[newprice].push_back(std::make_unique<Order>(existing)); 
        }
    }
}
void Orderbook::clearBook(){ // should make all maps 
    bids.clear();
    asks.clear();
    orders.clear(); 
    // make sure to clear other stats 
}
// constructor
Orderbook::Orderbook() 
{

}
// public functions

void Orderbook::processEvent(const Order& order) {
    switch(order.getActionChar()) {
        case 'A': insertOrder(order); break;
        case 'C': cancelOrder(order); break;
        case 'M': modifyOrder(order); break;
        case 'R': clearBook();        break;
        case 'T': break;  // trade, no book update
        case 'F': break;  // fill, no book update
        case 'N': break;  // none, no book update
    }
    timestamp = order.getTimestamp(); 
}

std::string Orderbook::getDisplaySpread() const
{
    
    if (!bids.empty() && !asks.empty())
    {
        auto p = (asks.begin()->first - bids.begin()->first) / 1e9; 
        std::ostringstream oss; 
        oss << "$" << std::fixed << std::setprecision(2) << p; 
        return oss.str(); ;
    }
    else return "N/A"; 
}

void Orderbook::print() const
{
    int levels = 10; 
    int count = 0; 
    //  iterates the first 10 entries of the asks map and stores their price and total size as pairs in the vector:
    // price, quantity
    std::vector<std::pair<int64_t, int64_t>> askLevels; // at the front of the asks map, we have the lowest asks 
                                                        // we would like to print the top 10 lowest asks in descending order
    std::vector<std::pair<int64_t, int64_t>> bidLevels; // at the front of the bids map, we have the highest bids
                                                        // we would like to print the top 10 highest bids in descending order
    std::cout << "ORDERBOOK: " << timestamp << std::endl;
    std::cout << "\033[31mASKS\033[0m" << std::endl;
    count = 0;
    for (const auto& [key, value] : asks) // key value refers to price, deque
    {
        if (count++ >= levels) break;
        int64_t size = 0;
        for (const auto& order : value) // order value refers to each order in the deque (value)
        {
            size += order->getQuantity(); 
        }
        askLevels.push_back({key, size});
    }
    for (int i = (int)askLevels.size() - 1; i >= 0; i--) {
        std::cout << "\033[31m"  
                  << std::setw(12) << std::fixed << std::setprecision(2)<< askLevels[i].first / 1e9
                  << std::setw(10) << askLevels[i].second
                  << "\033[0m\n";
    } 

    // spread
    std::cout << "spread: " << getDisplaySpread()  << std::endl;

    // print bids
    count = 0;
    std::cout << "\033[1;32mBIDS\033[0m" << std::endl;
    for (const auto& [key, value] : bids)
    {
        if (count++ >= levels) break;
        int64_t size = 0;
        for (const auto& order : value)
        {
            size += order->getQuantity(); 
        }
        bidLevels.push_back({key, size});
    }
    for (int i = 0; i < (int)bidLevels.size(); i++) {
        std::cout << "\033[1;32m"
                 << std::setw(12) << std::fixed << std::setprecision(2)<< bidLevels[i].first / 1e9
                  << std::setw(10) << bidLevels[i].second
                  << "\033[0m\n";
    } 
}

const std::map<int64_t, std::deque<std::unique_ptr<Order>>, std::greater<int64_t>>& Orderbook::getBids() const
{
    return bids; 
}
        
const std::map<int64_t, std::deque<std::unique_ptr<Order>>, std::less<int64_t>>& Orderbook::getAsks() const
{
    return asks; 
}

void Orderbook::LoadSnapshot(L2snapshot snapshot)
{
     // this is to get the L2snapshot into orderbook
    for (const auto [key, value]: snapshot.bids) // price, quantity
    {
        // Order newOrder(timestamp, Mbo_msg.order_id, Mbo_msg.size, Mbo_msg.price, Mbo_msg.side, Mbo_msg.action);
        Order o(snapshot.time, 0, value, key, 'B', 'A');
        processEvent(o); 
    }
    for (const auto [key, value]: snapshot.asks) // price, quantity
    {
        // Order newOrder(timestamp, Mbo_msg.order_id, Mbo_msg.size, Mbo_msg.price, Mbo_msg.side, Mbo_msg.action);
        Order o(snapshot.time, 0, value, key, 'A', 'A');
        processEvent(o); 
    } 
}


std::vector<std::pair<int64_t, int64_t>> Orderbook::getAskLevels() 
{
    int levels = 10; 
    int count = 0; 
    std::vector<std::pair<int64_t, int64_t>> askLevels; // at the front of the bids map, we have the highest bids

    for (const auto& [key, value] : asks) // key value refers to price, deque
    {
        if (count++ >= levels) break;
        int64_t size = 0;
        for (const auto& order : value) // order value refers to each order in the deque (value)
        {
            size += order->getQuantity(); 
        }
        askLevels.push_back({key, size});
    }

    return askLevels; 
}

std::vector<std::pair<int64_t, int64_t>> Orderbook::getBidLevels()
{
    int levels = 10; 
    int count = 0; 
    std::vector<std::pair<int64_t, int64_t>> bidLevels; // at the front of the bids map, we have the highest bids

    for (const auto& [key, value] : bids)
    {
        if (count++ >= levels) break;
        int64_t size = 0;
        for (const auto& order : value)
        {
            size += order->getQuantity(); 
        }
        bidLevels.push_back({key, size});
    }

    return bidLevels; 
}