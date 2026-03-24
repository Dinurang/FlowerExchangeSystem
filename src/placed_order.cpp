#include "../include/placed_order.h"
#include <string>
using namespace std;

// Constructor with priority sequence
PlacedOrder::PlacedOrder(string clientOrderID , string instrument, int side, double price, int quantity, int priority_sequence) 
: Order(clientOrderID, instrument, side, price, quantity, priority_sequence){
    
}

// Constructor without priority sequence (defaults to 0)
PlacedOrder::PlacedOrder(string clientOrderID , string instrument, int side, double price, int quantity) 
: Order(clientOrderID, instrument, side, price, quantity, 0){
    
}

PlacedOrder::~PlacedOrder(){

}


void PlacedOrder::setQuantity(int newQuantity){
    quantity = newQuantity;
}




