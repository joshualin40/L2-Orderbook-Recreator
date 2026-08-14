#include <databento/historical.hpp>
#include <databento/record.hpp>
#include <databento/symbology.hpp>
#include <iostream>
#include <fstream>
#include <string> 
#include <cstdint> // uint64_t, uint32_t, uint8_t, int64_t, int32_t etc. // also getenv
#include "helpers.h"
#include "order.h"
#include "eventlog.h"
#include "orderbook.h"
#include "L2snapshot.h"
#include <nlohmann/json.hpp>
#include "httplib.h"
#include <stdexcept>   // std::runtime_error
#include <cstdlib>     // std::getenv

namespace db = databento;
using json = nlohmann::json;

json getOrderbook(std::string date, std::string time, std::string ticker)
{
    // Step 1: User inputs Date, Time, and Ticker Symbol
    std::string enddate;
    uint64_t inputtimenanoseconds; 
    std::string startdate2, enddate2; 
    int year, month, day;   
    year = std::stoi(date.substr(0,4)); 
    month = std::stoi(date.substr(5,2));
    day = std::stoi(date.substr(8,2));

    enddate = date; 
    startdate2 = date; 
    enddate2 = date; 
    // convert inputtime to nanoseconds
    inputtimenanoseconds = toNanoSeconds(time);
    date += "T00:00:00";
    enddate += "T23:59:59"; 

    // Step 2: Extract the full day of trades of the ticker
    const char* apiKey = std::getenv("API_KEY");
    if (!apiKey) {
        throw std::runtime_error("Missing API Key");
    }
    std::string key((apiKey));

    auto client = db::Historical::Builder().SetKey(key).Build();
    auto store = client.TimeseriesGetRange(
            "XNAS.ITCH", {date, enddate}, {ticker}, db::Schema::Mbo, db::SType::RawSymbol,
    db::SType::InstrumentId, 0); // this TimeSeries processes the whole day
    

    std::map<uint64_t, L2snapshot> snapshots; // map of timestamp (stored as uint64_t, L2snapshots. in nanoseconds. 
    Orderbook Daybook; 
    Eventlog log; 
    uint64_t initialtime = 0; 
    constexpr uint64_t SNAPSHOT_INTERVAL_NS = 5 * 60 * 1'000'000'000ULL; // 5 minutes
    uint64_t currenttime = 0; 
    std::tm t{0, 0, 0, day, month-1, year - 1900}; // for getting time since epoch in nanoseconds at midnight of given date
    uint64_t tt_ns = static_cast<uint64_t>(timegm(&t)) * 1'000'000'000ULL;
    while (const db::Record* record = store.NextRecord()) {
        
        const auto& Mbo_msg = record->Get<db::MboMsg>();

            // Guard here before anything else
        if (Mbo_msg.price == INT64_MAX || Mbo_msg.size == 0) continue;

        uint64_t timestamp = Mbo_msg.hd.ts_event.time_since_epoch().count(); // convert ts_event from a DATABENTO type to the type
                                                                            // used in Order class, uint64_t
        Order newOrder(timestamp, Mbo_msg.order_id, Mbo_msg.size, Mbo_msg.price, Mbo_msg.side, Mbo_msg.action);
        Daybook.processEvent(newOrder);

        if (initialtime == 0) // capture initial time stamp 
        {
            initialtime = timestamp;
            currenttime = initialtime; 
        }
        if (timestamp >= currenttime)
        {
            L2snapshot snapshot;
            snapshot.takeSnapshot(Daybook);
            snapshot.time = timestamp - tt_ns;   // ns since midnight, actual capture time
            snapshots[snapshot.time] = snapshot; 
            do { currenttime += SNAPSHOT_INTERVAL_NS; } while (timestamp >= currenttime);
        }
    }

    // Step 3: create an Orderbook from a snapshot of the the most recent timestamp before the user specified time
    // stream the events from the timestamp to the userspecified tine to the Orderbook
    // output the orderbook. 
    
    // first find the latest timestamp. ie find the greatest L2Snapshot where snapshot.time < timesincenanoseconds
    uint64_t latest = 0; 
    uint64_t smallestdifference = UINT64_MAX; 
    for (const auto& [key, value] : snapshots)
    {
        if (key > inputtimenanoseconds) continue; // skip future snapshots
        uint64_t difference = inputtimenanoseconds - key;
        if (difference < smallestdifference) 
        {
            latest = key;
            smallestdifference = difference; 
        }
    }
    // then create Orderbook based on the timestamp, stream events from the time interval to input time, and print orderbook 
    startdate2 += timetoString(latest);
    enddate2 += timetoString(inputtimenanoseconds);
    auto store2 = client.TimeseriesGetRange(
            "XNAS.ITCH", {startdate2, enddate2}, {ticker}, db::Schema::Mbo, db::SType::RawSymbol,
    db::SType::InstrumentId, 0);

    Orderbook userbook; 
    L2snapshot usersnapshot = snapshots[latest];
    userbook.LoadSnapshot(usersnapshot); 
    
    Order lastorder(0, 0, 0, 0, 'N', 'N'); 
    while (const db::Record* record = store2.NextRecord()) {
        
        const auto& Mbo_msg = record->Get<db::MboMsg>();
        if (Mbo_msg.price == INT64_MAX || Mbo_msg.size == 0) continue;

        uint64_t timestamp = Mbo_msg.hd.ts_event.time_since_epoch().count(); // convert ts_event from a DATABENTO type to the type
                                                                            // used in Order class, uint64_t
        Order newOrder(timestamp, Mbo_msg.order_id, Mbo_msg.size, Mbo_msg.price, Mbo_msg.side, Mbo_msg.action);
        userbook.processEvent(newOrder);
        log.addOrder(newOrder);
        lastorder = newOrder; 
    }
    if (lastorder.getActionChar() == 'T' || lastorder.getActionChar() == 'F') // we want to simulate the corresponding cancel action
    { // we can do this by creating a synthetic order for the userbook to process
        Order newOrder(0, lastorder.getOrderID(), lastorder.getQuantity(), lastorder.getPrice(), lastorder.getSideChar(), 'C');
        userbook.processEvent(newOrder); 
    }
    userbook.print(); 
    log.print(); 


    // create JSON 
    json j; 

    std::vector<std::pair<int64_t, int64_t>> askLevels = userbook.getAskLevels(); 
    std::vector<std::pair<int64_t, int64_t>> bidLevels = userbook.getBidLevels(); 

    for (const auto& [price, qty] : askLevels)
        j["asks"].push_back({{"price", price / 1e9}, {"quantity", qty}});

    for (const auto& [price, qty] : bidLevels)
        j["bids"].push_back({{"price", price / 1e9}, {"quantity", qty}});

    return j;
}

int main() {

    httplib::Server svr;
    
    svr.Get("/orderbook", [](const httplib::Request &req, httplib::Response &res) {
        try {
            std::string frontdate, fronttime, frontticker; 
            res.set_header("Access-Control-Allow-Origin", "*");

            frontdate = req.get_param_value("date");
            fronttime = req.get_param_value("time");
            frontticker = req.get_param_value("ticker");


            json result = getOrderbook(frontdate, fronttime, frontticker); 

            std::string s = result.dump(); 
            std::ofstream filestream; 
            filestream.open("output.json");

            filestream << s; 
            res.set_content(s, "application/json");
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(e.what(), "text/plain");
        }
    });

    svr.listen("0.0.0.0", 8080);
}


