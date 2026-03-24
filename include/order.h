#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

class Order
{
private:
    string clientOrderID;
    string instrument;
    int side;
    double price;
    

protected:
    int quantity;
    int priority_sequence;
    
    
public:
    Order(string clientOrderID , string instrument, int side, double price, int quantity, int priority_sequence);
    
    virtual ~Order();

    string getClientOrderID() const;
    string getInstrument() const; 
    int getSide() const;
    double getPrice() const;
    int getQuantity() const;
    int getPrioritySequence() const;

};


#endif 