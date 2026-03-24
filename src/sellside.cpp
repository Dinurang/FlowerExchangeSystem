#include "../include/sellside.h"
#include <string>
#include <list>
using namespace std;


SellSide::SellSide(string instrument, int side) : BookSide(instrument, side) {
    // Call the base class constructor to initialize the instrument and side
}

SellSide::~SellSide(){

}


void SellSide::addOrder(PlacedOrder order){
    // SELL side: Sort by ASCENDING price (lowest first = most attractive)
    // Then by ASCENDING priority_sequence (earlier orders first = time priority)
    for (auto it = orderList.begin(); it != orderList.end(); ++it) {
        if (order.getPrice() < it->getPrice()) {
            // Insert the new order before the current order (lower price = more attractive)
            orderList.insert(it, order);
            return;
        } else if (order.getPrice() == it->getPrice()) {
            // If prices are equal, compare priority sequence for time priority
            if (order.getPrioritySequence() < it->getPrioritySequence()) {
                // Insert the new order before the current order (lower sequence = earlier = higher priority)
                orderList.insert(it, order);
                return;
            }
        }
    }
    orderList.push_back(order);

   
}