#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include "buyside.h"
#include "sellside.h"
#include <string>
using namespace std;   

class OrderBook
{
    private:
    BuySide buySide;
    SellSide sellSide;
    string instrument;

    public:
    OrderBook(string instrument);
    ~OrderBook();

    BuySide& getBuySide();
    SellSide& getSellSide();

    
};

#endif


