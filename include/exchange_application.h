#ifndef EXCHANGE_APPLICATION_H
#define EXCHANGE_APPLICATION_H

#include "orderbook.h"
#include "placed_order.h"
#include "executed_order.h"
#include "execution_report.h"
#include <map>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class ExchangeApplication
{
private:
    map<string, OrderBook*> orderBooks;  // Map of Flower instrument to its OrderBook
    ExecutionReport executionReport;
    int executedOrderCounter;  // Counter for generating unique order ID which is client- independent
    int prioritySequenceCounter;  // Counter for generating order priority sequence (time ordering)

    // Private helper methods
    OrderBook* getOrCreateOrderBook(string instrument);
    string getOrCreateOrderID(string clientOrderID);
    void matchOrders(PlacedOrder& incomingOrder, OrderBook* orderBook);
    void executeMatch(PlacedOrder& incomingOrder, PlacedOrder& bookOrder, 
                     OrderBook* orderBook, double executionPrice, int executionQty);
    string generateOrderID();
    int generatePrioritySequence();  // Generate next priority sequence
    string getCurrentTimestamp();

public:
    ExchangeApplication();
    ~ExchangeApplication();

    // Main business logic method - process incoming order
    void processPlacedOrder(PlacedOrder incomingOrder);

    // Generate execution report (write to CSV)
    void generateExecutionReport();

    // Getter for execution report
    ExecutionReport& getExecutionReport();
};

#endif
