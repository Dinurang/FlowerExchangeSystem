#include "../include/execution_report.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

ExecutionReport::ExecutionReport(){

}


ExecutionReport::~ExecutionReport(){

}


void ExecutionReport::addExecutedOrder(ExecutedOrder newExecutedOrder){
    executedOrderList.push_back(newExecutedOrder);
}

void ExecutionReport::addRejectedExecutedOrder(ExecutedOrder newRejectedExecutedOrder){
    rejectedExecutedOrderList.push_back(newRejectedExecutedOrder);
}

vector<ExecutedOrder> ExecutionReport::getExecutedOrderList() const {
    return executedOrderList;
}

vector<ExecutedOrder> ExecutionReport::getRejectedExecutedOrderList() const {
    return rejectedExecutedOrderList;
}


// Display execution report summary
void ExecutionReport::displayExecutionReportSummary()
{
    vector<ExecutedOrder> executedOrders = this->getExecutedOrderList();
    
    cout << "\n" << string(70, '=') << endl;
    cout << "EXECUTION REPORT SUMMARY" << endl;
    cout << string(70, '=') << endl;
    
    
    for (const auto& order : executedOrders) {
        cout << "OrderID: " << order.getOrderID()
             << " | ClientOrderID: " << order.getClientOrderID()
             << " | Instrument: " << order.getInstrument()
             << " | Side: " << order.getSide()
             << " | Qty: " << order.getQuantity()
             << " @ "   << order.getTransactionPrice()
             << " | Status: " << order.getStatus()
             << " | Time: " << order.getTransactionTime() << endl;
    }
    
    cout << string(70, '=') << endl;


    vector<ExecutedOrder> rejectedOrders = this->getRejectedExecutedOrderList();
    if (!rejectedOrders.empty()) {
        for (const auto& order : rejectedOrders) {
            cout << "ClientOrderID: " << order.getClientOrderID()
                 << " | Instrument: " << order.getInstrument()
                 << " | Side: " << order.getSide()
                 << " | Qty: " << order.getQuantity()
                 << " @ "   << order.getPrice()
                 <<" | Status: "<< order.getStatus()
                 << " | Reason: " << order.getReason() 
                 << " | Time: " << order.getTransactionTime() << endl;
        }
        cout << string(70, '=') << endl;
    }
}

void ExecutionReport::toCSV(const vector<ExecutedOrder>& executedOrderList, int repType)
{   
    if (repType == 1){
        string filePath = "./reports/execution_rep.csv";
        ofstream outFile(filePath); 
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file for writing: " << filePath << endl;
            return;
        }      
        outFile << "OrderID,ClientOrderID,Instrument,Side,Price,Quantity,Status,TransactionTime,TransactionPrice\n";
        for (const auto& order : executedOrderList) {
            outFile << order.getOrderID() << ","
                    << order.getClientOrderID() << ","
                    << order.getInstrument() << ","
                    << order.getSide() << ","
                    << fixed << setprecision(2) << order.getPrice() << ","
                    << order.getQuantity() << ","
                    << order.getStatus() << ","
                    << order.getTransactionTime() << ","
                    << fixed << setprecision(2) << order.getTransactionPrice() 
                    << "\n";
        }
        outFile.close();



    }else if(repType==2){
        string filePath = "./reports/rejected_execution_rep.csv";
    ofstream outFile(filePath); 
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file for writing: " << filePath << endl;
        return;
    }      
    outFile << "OrderID,ClientOrderID,Instrument,Side,Price,Quantity,Status,Reason,TransactionTime,TransactionPrice\n";
    for (const auto& order : executedOrderList) {
        outFile << order.getOrderID() << ","
                << order.getClientOrderID() << ","
                << order.getInstrument() << ","
                << order.getSide() << ","
                << fixed << setprecision(2) << order.getPrice() << ","
                << order.getQuantity() << ","
                << order.getStatus() << ","
                << order.getReason() << ","
                << order.getTransactionTime() << ","
                << fixed << setprecision(2) << order.getTransactionPrice() 
                << "\n";
    }
    outFile.close();
    }
    

}



