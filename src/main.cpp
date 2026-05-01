#include <databento/historical.hpp>
#include <databento/record.hpp>
#include <databento/symbology.hpp>
#include <iostream>
#include <string> 
#include <cstdint> // uint64_t, uint32_t, uint8_t, int64_t, int32_t etc. // also getenv
#include "inputvalidation.h"
#include "order.h"
#include "eventlog.h"
#include "orderbook.h"

namespace db = databento;
// Schema is the format the data comes in 

int main() {
    // Step 1: User inputs Date and Ticker Symbol
    std::string startdate, enddate, ticker;
    int year, month, day;   
    while (true) // loop asking user for the date until valid date and format is given 
    {
    std::cout << "Enter date between 2018-05-01 and 2026-04-27 in YYYY-MM-DD format: ";
        std::cin >> startdate;
        year = std::stoi(startdate.substr(0,4)); 
        month = std::stoi(startdate.substr(5,2));
        day = std::stoi(startdate.substr(8,2));
        if (isWeekday(year, month, day) && inRange(year, month, day)) 
            break;
        if (!isWeekday(year, month, day))
        {
            std::cout << "Invalid date: Not a weekday" << std::endl;
        }
        if (!inRange(year, month, day))
        {
            std::cout << "Invalid date: Not in range" << std::endl;;
        }
    }
    enddate = startdate; 
    startdate += "T00:00:00";
    enddate += "T23:59:59"; 
    std::cout << "Enter Ticker Symbol: "; // here create another loop asking user for a ticker until a valid ticker is given 
                                        // should create another header/cpp file for the validation of ticker 
    std::cin >> ticker;





    // Step 2: Extract the full day of trades of the ticker
    const char* apiKey = std::getenv("API_KEY");
    if (!apiKey) {
        throw("Missing API Key");
    }
    std::string key(apiKey);
    auto client =
        db::Historical::Builder().SetKey(key).Build();
    auto store = client.TimeseriesGetRange(
            "XNAS.ITCH", {startdate, enddate}, {ticker}, db::Schema::Mbo, db::SType::RawSymbol,
    db::SType::InstrumentId, 0 /* max amount of records to return */); //using XNAS.ITCH (Nasdaq TotalView-ITCH). start: 2018-05-01, 2026-04-27
    
    auto record_count = client.MetadataGetRecordCount(
            "XNAS.ITCH", {startdate, enddate}, {ticker}, db::Schema::Mbo); // returns the number of records in the day


    
    int numEvents = 0; 
    std::cout << record_count << " total events..." << std::endl;
    std::cout << "How many events to display: ";
    std::cin >> numEvents; 

    Orderbook book; 
    Eventlog log; 
    int count = 0; 
    while (const db::Record* record = store.NextRecord()) {
        
        const auto& Mbo_msg = record->Get<db::MboMsg>();

            // Guard here before anything else
        if (Mbo_msg.price == INT64_MAX || Mbo_msg.size == 0) continue;
        uint64_t timestamp = Mbo_msg.hd.ts_event.time_since_epoch().count(); // convert ts_event from a DATABENTO type to the type
                                                                            // used in Order class, uint64_t
    
        Order newOrder(timestamp, Mbo_msg.order_id, Mbo_msg.size, Mbo_msg.price, Mbo_msg.side, Mbo_msg.action);
        if (Mbo_msg.flags.IsSnapshot()) { // if its a snap50 shot, only add it to the book but not the event log. IsSnapShot() is a flag indicated whether
            // the record is a snapshot from the previous day or from current day. 
            book.processEvent(newOrder);   // seed book
        } else {
            book.processEvent(newOrder);
            log.addOrder(newOrder);
            if (count++ >= numEvents) break;
        }
    }
    // display orderbook
    log.print();
    book.print(); 
    
    
}


// cmake -S . -B build
// cmake --build build --parallel
// ./build/example

// cmake --build build --parallel && ./build/example