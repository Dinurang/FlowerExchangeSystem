#include "../include/executed_order.h"
#include <string>
using namespace std;

ExecutedOrder::ExecutedOrder(string orderID , string clientOrderID , string instrument, int side,
         double price, int quantity, string status, string reason, string transactionTime, double transactionPrice) 
         : Order(clientOrderID, instrument, side, price, quantity, 0){
    this->orderID = orderID;
    this->status = status;
    this->reason = reason;
    this->transactionTime = transactionTime;
    this->transactionPrice = transactionPrice;
}

ExecutedOrder::~ExecutedOrder(){

}



string ExecutedOrder::getOrderID() const {
    return orderID;
}
string ExecutedOrder::getStatus() const {
    return status;
}
string ExecutedOrder::getReason() const {
    return reason;
}
string ExecutedOrder::getTransactionTime() const {
    return transactionTime;
}

double ExecutedOrder::getTransactionPrice() const {
    return transactionPrice;
}