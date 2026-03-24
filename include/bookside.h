#ifndef BOOKSIDE_H
#define BOOKSIDE_H
#include <string>
#include <list>
#include "placed_order.h"
using namespace std;


class BookSide
{
    private:
    string instrument;
    int side;

    protected:
    list<PlacedOrder> orderList;

    public:
    BookSide(string instrument, int side);
    ~BookSide();

    virtual void addOrder(PlacedOrder order) = 0;
    void removeOrder(PlacedOrder order);
    void updateOrder(PlacedOrder order, int newQuantity);
    list<PlacedOrder>& getOrderList();

    

};



#endif