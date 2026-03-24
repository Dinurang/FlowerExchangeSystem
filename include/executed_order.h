#ifndef EXECUTED_ORDER_H
#define EXECUTED_ORDER_H
#include "order.h"

class ExecutedOrder : public Order
{ 
    private:
    string orderID;
    string status;
    string reason;
    string transactionTime;
    double transactionPrice;

    public:
    ExecutedOrder(string orderID , string clientOrderID , string instrument, int side,
         double price, int quantity, string status, string reason, string transactionTime, double transactionPrice);
    ~ExecutedOrder();

    string getOrderID() const;
    string getStatus() const;
    string getReason() const;
    string getTransactionTime() const;
    double getTransactionPrice() const;

} ;



#endif