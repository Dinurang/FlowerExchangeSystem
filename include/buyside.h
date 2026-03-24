#ifndef BUYSIDE_H
#define BUYSIDE_H
#include "bookside.h"

class BuySide : public BookSide
{
    public:
    BuySide(string instrument, int side);
    ~BuySide();

    void addOrder(PlacedOrder order) override;

    
};

#endif