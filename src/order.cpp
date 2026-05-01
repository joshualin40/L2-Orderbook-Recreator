#include "order.h"


// constructor
Order::Order(uint64_t timestamp_, uint64_t orderid_, uint32_t quantity_, int64_t price_, char side_, char action_) :
timestamp(timestamp_), orderid(orderid_), quantity(quantity_), price(price_), side(side_), action(action_) {}
// string getters (for display)
std::string Order::getTimestamp() const {
    std::time_t seconds = timestamp / 1'000'000'000; // convert nanoseconds to seconds
    uint64_t millis  = (timestamp % 1'000'000'000) / 1'000'000;
    std::tm* t = std::gmtime(&seconds); 
    std::ostringstream oss;
    oss << std::put_time(t, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << std::to_string(millis);
    return oss.str();
}
std::string Order::getSession() const
{
    // convert nanoseconds to seconds
    std::time_t seconds = timestamp / 1'000'000'000;
    std::tm* t = std::gmtime(&seconds); //gm time takes pointer and returns pointer 
    auto hours = t->tm_hour;
    auto minutes = t->tm_min; 
    auto totalminutes = hours * 60 + minutes;

    // SYSTEM is before 8:00 
    if (totalminutes < 8 * 60)
        return "SYS";
    else if (totalminutes < 13 * 60 + 30)
        return "PRESESSION";
    else if (totalminutes < 20 * 60)
        return "MARKET"; 
    else return "AFTERHOURS";
}
std::string Order::getAction() const
{
    switch (action)
    {
        case ('A'):
            return "Add";
            break;
        case ('C'):
            return "Cancel";
            break;
        case ('M'):
            return "Modify";
            break;
        case ('R'):
            return "Clear Book";
            break;
        case ('T'):
            return "Trade";
            break;
        case ('F'):
            return "Fill";
            break;
        case ('N'):
            return "None";
          default:
            return "Error"; // what should i do here?
    }
}
std::string Order::getSide() const
{
    switch (side)
    {
        case ('A'):
            return "Ask";
            break;
        case ('B'):
            return "Bid";
            break;
        case ('N'):
            return "None";
            break;
        default:
            return "Error"; // what should i do here?
    }
}
std::string Order::getDisplayOrderID() const
{
    if (action == 'R') return "N/A";

    return std::to_string(orderid);
}
std::string Order::getDisplayPrice() const // we want the price to 3 digits with 2 decimal places 
{
    if (action == 'R') return "N/A";
    auto p = price / 1e9; 
    std::ostringstream oss; 
    oss << "$" << std::fixed << std::setprecision(5) << p; 
    return oss.str(); 
}
std::string Order::getDisplayQuantity() const
{
    if (action == 'R') return "N/A";

    return std::to_string(quantity);
}
// raw data type getters (for book calculations)
char Order::getActionChar() const
{
    return action; 
}
char Order::getSideChar() const
{
    return side; 
}
uint64_t Order::getOrderID() const
{
    return orderid;
}
int64_t Order::getPrice() const
{
    return price; 
}
uint32_t Order::getQuantity() const {
    return quantity; 
}
// setters
void Order::setPrice(int64_t new_price)
{
    price = new_price; 
}
void Order::setQuantity(uint32_t new_quantity) 
{
    quantity = new_quantity; 
}