#ifndef EXECUTION_REPORT_H
#define EXECUTION_REPORT_H

#include "executed_order.h"
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

class ExecutionReport
{
    private:
    vector<ExecutedOrder> executedOrderList = {};
    vector<ExecutedOrder> rejectedExecutedOrderList = {};

    public:
    ExecutionReport();
    ~ExecutionReport();

    void addExecutedOrder(ExecutedOrder newExecutedOrder);
    void addRejectedExecutedOrder(ExecutedOrder newRejectedExecutedOrder);
    vector<ExecutedOrder> getExecutedOrderList() const;
    vector<ExecutedOrder> getRejectedExecutedOrderList() const;
    void displayExecutionReportSummary();
    void toCSV(const vector<ExecutedOrder>& executedOrderList, int repType);
    
    
} ;


#endif





