#ifndef SELLSIDE_H
#define SELLSIDE_H
#include "bookside.h"

class SellSide : public BookSide
{
    public:
    SellSide(string instrument, int side);
    ~SellSide();

    void addOrder(PlacedOrder order) override;

    
};

#endif