#ifndef PLACED_ORDER_H
#define PLACED_ORDER_H
#include "order.h"


class PlacedOrder : public Order
{ 
    public:
    PlacedOrder(string clientOrderID , string instrument, int side, double price, int quantity, int priority_sequence);
    PlacedOrder(string clientOrderID , string instrument, int side, double price, int quantity);
    ~PlacedOrder();

    void setQuantity(int newQuantity);


} ;




#endif