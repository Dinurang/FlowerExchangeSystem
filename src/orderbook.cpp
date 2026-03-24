#include "../include/orderbook.h"
#include <string>
using namespace std;

OrderBook::OrderBook(string instrument): buySide(instrument, 1), sellSide(instrument, 2)
{
    this->instrument = instrument;
}

OrderBook::~OrderBook()
{
}

BuySide& OrderBook::getBuySide()
{
    return buySide;
}

SellSide& OrderBook::getSellSide()
{
    return sellSide;
}


