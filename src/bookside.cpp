#include "../include/bookside.h"
#include <string>
#include <list>
using namespace std;


BookSide::BookSide(string instrument, int side){
    this->instrument = instrument;
    this->side = side;
    this->orderList = list<PlacedOrder>();
}

BookSide::~BookSide(){

}


//virtual functions are implemented in child classes 

//concrete implementations
void BookSide::removeOrder(PlacedOrder order){
    for (auto it = orderList.begin(); it != orderList.end(); ) {
    if (it->getClientOrderID() == order.getClientOrderID()) {
        // erase() removes the element and returns the NEXT valid iterator
        it = orderList.erase(it); 
        return; // Exit after removing the order
    } else {
        ++it; // Only move forward if we didn't delete anything
    }
}
    
}


void BookSide::updateOrder(PlacedOrder order, int newQuantity){
    for (auto& o : orderList) {
        if (o.getClientOrderID() == order.getClientOrderID()) {
            o.setQuantity(newQuantity);
            return; // Exit after updating the order
        }
    }
    

}

list<PlacedOrder>& BookSide::getOrderList(){
    return orderList;
}

