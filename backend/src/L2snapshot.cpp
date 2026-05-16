#include "orderbook.h"
#include "L2snapshot.h"

void L2snapshot::takeSnapshot(Orderbook const& orderbook_)
{
    for (const auto& [price, orders] : orderbook_.getBids()) // remember that orders is a deque 
    {
        for (const auto& item : orders) {
            bids[price] += item->getQuantity(); 
        }
    }

    for (const auto& [price, orders] : orderbook_.getAsks()) // remember that orders is a deque 
    {
        for (const auto& item : orders) {
            asks[price] += item->getQuantity(); 
        }
    }
}

void L2snapshot::print() const
{
    int count = 0; 
    std::cout << "START OF NEW ORDERBOOK AT " << getTimeStamp() << std::endl;
     std::cout << "\033[31mASKS\033[0m" << std::endl;

    std::vector<std::pair<int64_t, int64_t>> bidLevels;
    std::vector<std::pair<int64_t, int64_t>> askLevels;
    for (const auto [price, quantity]: bids)
        {
            if (count++ >= 10) break;
            bidLevels.push_back({price, quantity}); 
        }

    for (int i = 0 ; i <  (int)bidLevels.size();  i++)
    {
        std::cout << "\033[31m"  
                  << std::setw(12) << std::fixed << std::setprecision(2)<< bidLevels[i].first / 1e9
                  << std::setw(10) << bidLevels[i].second
                  << "\033[0m\n";
    }
    

    count = 0; 
    std::cout << "\033[1;32mBIDS\033[0m" << std::endl;
    for (const auto [price, quantity]: asks)
        {
            if (count++ >= 10) break;
            askLevels.push_back({price, quantity});
        }
    for (int i = (int)askLevels.size() - 1; i >= 0; i--) {
         std::cout << "\033[1;32m"
                  << std::setw(12) << std::fixed << std::setprecision(2)<< askLevels[i].first / 1e9
                  << std::setw(10) << askLevels[i].second
                  << "\033[0m\n";
    }
}

std::string L2snapshot::getTimeStamp() const
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