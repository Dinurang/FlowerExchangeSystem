#include "../include/order.h"
#include <string>
using namespace std;

Order::Order(string clientOrderID , string instrument, int side, double price, int quantity, int priority_sequence)
{
    this->clientOrderID = clientOrderID;
    this->instrument = instrument;
    this->side = side;
    this->price = price;
    this->quantity = quantity;
    this->priority_sequence = priority_sequence;

}

Order::~Order(){

}


string Order::getClientOrderID() const {
    return clientOrderID;
}
string Order::getInstrument() const {
    return instrument;
}   
int Order::getSide() const {
    return side;
}
double Order::getPrice() const {
    return price;
}   
int Order::getQuantity() const {
    return quantity;
}
int Order::getPrioritySequence() const {
    return priority_sequence;
}   





