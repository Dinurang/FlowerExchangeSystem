#include "../include/exchange_application.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
using namespace std;

unordered_map<string, string> mp;



ExchangeApplication::ExchangeApplication() : executedOrderCounter(1), prioritySequenceCounter(0){
    //initialize counters for order IDs and priority sequences
}



ExchangeApplication::~ExchangeApplication()
{   // Clean up all order books
    for (auto& pair : orderBooks) {
        delete pair.second;
    }
}


//========== Get or create an order book for a specific instrument =================================
 
OrderBook* ExchangeApplication::getOrCreateOrderBook(string instrument)
{
    if (orderBooks.find(instrument) == orderBooks.end()) {
        // Order book doesn't exist, create it
        orderBooks[instrument] = new OrderBook(instrument);
    }
    return orderBooks[instrument];
}

// ================ helper function for reuse of OrderID ==========================================
string ExchangeApplication::getOrCreateOrderID(string clientOrderID)
{
    // If ClientOrderID already has an orderID, REUSE it
    if (mp.find(clientOrderID) != mp.end()) {
        return mp[clientOrderID];  
    } else {
        // First time seeing this ClientOrderID, generate new orderID
        string orderID = generateOrderID();
        mp[clientOrderID] = orderID;  // Store in the unordered map
        return orderID;
    }
}



// ===================== Generate a unique order ID - but this is not applicable for csv reads======
string ExchangeApplication::generateOrderID()
{
    return "ord" + to_string(executedOrderCounter++);
}



// ============== Generate next priority sequence for time ordering ===============================
int ExchangeApplication::generatePrioritySequence()
{
    return prioritySequenceCounter++;
}


//================ Get current timestamp in format: YYYY-MM-DD HH:MM:SSs ===========================
string ExchangeApplication::getCurrentTimestamp() {
    using namespace std::chrono;

    // 1. Get current time with millisecond precision
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // 2. Convert to time_t for the YYYYMMDD-HHMMSS part
    time_t timer = system_clock::to_time_t(now);
    tm* bt = localtime(&timer);

    // 3. Format everything into the stringstream
    stringstream ss;
    ss << put_time(bt, "%Y%m%d-%H%M%S"); // Standard parts
    ss << '.' << setfill('0') << setw(3) << ms.count(); // The .sss part

    return ss.str();
}


//=================================================================================================

// Execute a match between incoming order and book order
void ExchangeApplication::executeMatch(PlacedOrder& incomingOrder, PlacedOrder&  bookOrder ,
                                       OrderBook* , double executionPrice, int executionQty)
{
    string incomingOrderID = getOrCreateOrderID(incomingOrder.getClientOrderID());
    string timestamp = getCurrentTimestamp();
    
    // Status codes: 0 = Filled, 1 = Partially Filled, 2 = Rejected
    string incomingStatus = (incomingOrder.getQuantity() == executionQty) ? "Fill" : "Pfill"; 
    
    ExecutedOrder incomingExecutedOrder(
        incomingOrderID,
        incomingOrder.getClientOrderID(),
        incomingOrder.getInstrument(),
        incomingOrder.getSide(),
        executionPrice,
        executionQty,
        incomingStatus,
        "Order Matched",
        timestamp,
        executionPrice
    );
    
    // Add incoming order to execution report
    executionReport.addExecutedOrder(incomingExecutedOrder);
    

//----------------------------------


    // Create ExecutedOrder for the book order (counter-party)
    string bookOrderID = getOrCreateOrderID(bookOrder.getClientOrderID());
    // Determine status for book order based on whether it's completely filled
    string bookOrderStatus = (bookOrder.getQuantity() == executionQty) ? "Fill" : "Pfill";
    
    ExecutedOrder bookExecutedOrder(
        bookOrderID,
        bookOrder.getClientOrderID(),
        bookOrder.getInstrument(),
        bookOrder.getSide(),
        executionPrice,
        executionQty,
        bookOrderStatus,
        "Order Matched",
        timestamp,
        executionPrice
    );
    // Add book order to execution report
    executionReport.addExecutedOrder(bookExecutedOrder);

}

//================================================================================================


// Match orders from incoming order against the order book
void ExchangeApplication::matchOrders(PlacedOrder& incomingOrder, OrderBook* orderBook)
{
    int remainingQty = incomingOrder.getQuantity();
    BuySide& buySide = orderBook->getBuySide();
    SellSide& sellSide = orderBook->getSellSide();
    bool orderMatched = false;
    
    if (incomingOrder.getSide() == 1) {
        // Incoming is BUY - try to match with SELL orders (lowest price first)
        list<PlacedOrder>& sellOrders = sellSide.getOrderList();
        
        auto it = sellOrders.begin();
        while (it != sellOrders.end() && remainingQty > 0) {
            PlacedOrder& sellOrder = *it;
            
            // Check if price matches: incoming BUY price >= SELL order price
            if (incomingOrder.getPrice() >= sellOrder.getPrice()) {
                // Match found
                int matchQty = min(remainingQty, sellOrder.getQuantity());
                executeMatch(incomingOrder, sellOrder, orderBook, sellOrder.getPrice(), matchQty);
                
                remainingQty -= matchQty;
                
                // Update the sell order quantity or remove if fully filled
                if (matchQty == sellOrder.getQuantity()) {
                    it = sellOrders.erase(it);  // Remove completely filled order
                } else {
                    sellOrder.setQuantity(sellOrder.getQuantity() - matchQty);
                    ++it;
                }
            orderMatched = 1;
            } else {
                
                break;  // No more matches possible (orders sorted by price)
            }
        }
        
        // Add remaining quantity to BUY side if any
        if (remainingQty > 0) {
            incomingOrder.setQuantity(remainingQty);
            buySide.addOrder(incomingOrder);
            
            // If no match occurred at all, add NEW order to execution report
            if (!orderMatched) {
                string orderID = getOrCreateOrderID(incomingOrder.getClientOrderID());
                string timestamp = getCurrentTimestamp();
                
                ExecutedOrder newOrder(
                    orderID,
                    incomingOrder.getClientOrderID(),
                    incomingOrder.getInstrument(),
                    incomingOrder.getSide(),
                    incomingOrder.getPrice(),
                    remainingQty,
                    "NEW",  // Status: NEW
                    "Order Added to Book",
                    timestamp,
                    incomingOrder.getPrice()
                );
                
                executionReport.addExecutedOrder(newOrder);
            }
     }
        
    } else if (incomingOrder.getSide() == 2) {
        // Incoming is SELL - try to match with BUY orders (highest price first)
        list<PlacedOrder>& buyOrders = buySide.getOrderList();
        
        auto it = buyOrders.begin();
        while (it != buyOrders.end() && remainingQty > 0) {
            PlacedOrder& buyOrder = *it;
            
            // Check if price matches: BUY order price >= incoming SELL price
            if (buyOrder.getPrice() >= incomingOrder.getPrice()) {
                // Match found
                int matchQty = min(remainingQty, buyOrder.getQuantity());
                executeMatch(incomingOrder, buyOrder, orderBook, buyOrder.getPrice(), matchQty);
                
                remainingQty -= matchQty;
                
                // Update the buy order quantity or remove if fully filled
                if (matchQty == buyOrder.getQuantity()) {
                    it = buyOrders.erase(it);  // Remove completely filled order
                } else {
                    buyOrder.setQuantity(buyOrder.getQuantity() - matchQty);
                    ++it;
                }orderMatched = 1;
            } else {

                break;  // No more matches possible (orders sorted by price)
            }
        }
        
        // Add remaining quantity to SELL side if any
        if (remainingQty > 0) {
            incomingOrder.setQuantity(remainingQty);
            sellSide.addOrder(incomingOrder);

            // If no match occurred at all, add NEW order to execution report
            if (!orderMatched) {
                string orderID = getOrCreateOrderID(incomingOrder.getClientOrderID());
                string timestamp = getCurrentTimestamp();
                
                ExecutedOrder newOrder(
                    orderID,
                    incomingOrder.getClientOrderID(),
                    incomingOrder.getInstrument(),
                    incomingOrder.getSide(),
                    incomingOrder.getPrice(),
                    remainingQty,
                    "NEW",  // Status: NEW
                    "Order Added to Book",
                    timestamp,
                    incomingOrder.getPrice()
                );
                
                executionReport.addExecutedOrder(newOrder);
            }

        }
    }
}



//================= Main business logic - process a placed order ====================================
//===================================================================================================

void ExchangeApplication::processPlacedOrder(PlacedOrder incomingOrder)
{
    string reason = "";
    vector<string> validInstruments = {"Rose", "Tulip", "Lotus","Orchid", "Lavender"};
    
    try {
        // VALIDATION: Check for required fields and order constraints
        
        // 1. Check if ClientOrderID is not empty
        if (incomingOrder.getClientOrderID().empty()) {
            throw runtime_error("Invalid ClientOrderID");
        }
        
        // 2. Check if Instrument is valid 
        if (incomingOrder.getInstrument().empty()) {
            throw runtime_error("Invalid Instrument");
        }
        if (incomingOrder.getInstrument() != "Rose" && incomingOrder.getInstrument() != "Tulip" && incomingOrder.getInstrument() != "Lotus"
            && incomingOrder.getInstrument() != "Orchid" && incomingOrder.getInstrument() != "Lavender") {
            throw runtime_error("Invalid Instrument");
        }
        
        // 3. Check if Side is valid (1=BUY or 2=SELL)
        int side = incomingOrder.getSide();
        if (side != 1 && side != 2) {
            throw runtime_error("Invalid side");
        }
        
        // 4. Check if Price is greater than 0
        if (incomingOrder.getPrice() <= 0) {
            throw runtime_error("Invalid Price");
        }
        
        // 5. Check if Quantity is a multiple of 10
        if (incomingOrder.getQuantity() % 10 != 0) {
            throw runtime_error("Quantity must be a multiple of 10");
        }
        
        // 6. Check if Quantity is within valid range (min=10, max=1000)
        int qty = incomingOrder.getQuantity();
        if (qty < 10 || qty > 1000) {
            throw runtime_error("Quantity must be between 10 and 1000");
        }
        
        // VALIDATION PASSED - Process the order
        
        // Generate priority sequence for this order (time stamp)
        int prioritySequence = generatePrioritySequence();
        
        // Create a new order with the priority sequence
        PlacedOrder orderedOrder(
            incomingOrder.getClientOrderID(),
            incomingOrder.getInstrument(),
            incomingOrder.getSide(),
            incomingOrder.getPrice(),
            incomingOrder.getQuantity(),
            prioritySequence
        );
        
        // Get or create order book for this instrument
        OrderBook* orderBook = getOrCreateOrderBook(orderedOrder.getInstrument());
        
        // Try to match the order
        matchOrders(orderedOrder, orderBook);
        

    } catch (const exception& e) {
        // Capture the reason for rejection from the exception
        reason = e.what();
        
        // Create rejected order
        string orderID = getOrCreateOrderID(incomingOrder.getClientOrderID());
        ExecutedOrder rejectedOrder(
            orderID,
            incomingOrder.getClientOrderID(),
            incomingOrder.getInstrument(),
            incomingOrder.getSide(),
            incomingOrder.getPrice(),
            incomingOrder.getQuantity(),
            "Reject",  // Status: Reject
            reason,
            getCurrentTimestamp(),
            incomingOrder.getPrice()
        );

        executionReport.addExecutedOrder(rejectedOrder);
        executionReport.addRejectedExecutedOrder(rejectedOrder);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generate execution report and write to CSV
void ExchangeApplication::generateExecutionReport()
{
    cout << "\n=== Generating Execution Report ===" << endl;
    vector<ExecutedOrder> report = executionReport.getExecutedOrderList();
    executionReport.toCSV(report,1);
    cout  <<"\n=== Generating Rejected Execution Report ===" << endl;
    vector<ExecutedOrder> rejectedReport = executionReport.getRejectedExecutedOrderList();
    executionReport.toCSV(rejectedReport,2);
}



// Getter for execution report
ExecutionReport& ExchangeApplication::getExecutionReport()
{
    return executionReport;
}
