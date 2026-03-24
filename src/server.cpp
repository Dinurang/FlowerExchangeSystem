#include <cstring> // Required for std::memset
#include <cstdlib> // Required for exit(EXIT_FAILURE)
#include "../include/server.h"
#include "../include/socket.h"
#include "../include/exchange_application.h"
#include "../include/execution_report.h"
#include "../include/placed_order.h"
#include "../include/gui.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
using namespace std;

#include <chrono>
using namespace std::chrono;

//global variables/ data structures
list<vector<string>> extreme_errors_report = {};

list<vector<string>> extreme_errors_rejected_report = {};

int errorIdentifier;

string curr_time;


//=============== Helper function to trim leading and trailing whitespace=========================
//  (spaces, tabs, newlines)

string trim(const string& str) {
    const string whitespace = " \t\r\n";
    
    // Find the first character that is NOT whitespace
    size_t start = str.find_first_not_of(whitespace);
    
    // If the string is entirely whitespace (or empty), return an empty string
    if (start == string::npos) {
        return "";
    }
    
    // Find the last character that is NOT whitespace
    size_t end = str.find_last_not_of(whitespace);
    
    // Extract and return the substring containing the actual data
    return str.substr(start, end - start + 1);
}

//========= Helper function Get current timestamp in format: YYYY-MM-DD HH:MM:S=====================

string getCurrentTimeStamp() {
    
    // 1. Get current time with millisecond precision
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // 2. Convert to time_t for the YYYYMMDD-HHMMSS part
    time_t timer = system_clock::to_time_t(now);
    tm* bt = localtime(&timer);

    // 3. Format everything into the stringstream
    stringstream ss;
    ss << put_time(bt, "%Y%m%d-%H%M%S"); // Standard parts
    ss << '.' << setfill('0') << setw(3) << ms.count(); // The .sss part

    return ss.str();
}


//========== 2 helper functions to append to csv ===========================================================

void appendMultipleOrders_report(const string& filePath, 
                         const list<vector<string>>& Orders) {
    
    ofstream csvFile(filePath, ios::app);
    
    if (!csvFile.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }
    
    for (const auto& order :  Orders) {
    
        csvFile << order[0] << "," << order[1] << ","
                << order[2] << "," << order[3] << ","
                << order[4] << ","<<order[5] << "," 
                <<order[6] << "," <<order[7] << "," 
                <<order[8] <<  ","  << endl;
        
    }
    
    csvFile.close();
}

void appendMultipleOrders_rejected_report(const string& filePath, 
                         const list<vector<string>>& Orders) {
    
    ofstream csvFile(filePath, ios::app);
    
    if (!csvFile.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }
    
    for (const auto& order :  Orders) {
    
        csvFile << order[0] << "," << order[1] << ","
                << order[2] << "," << order[3] << ","
                << order[4] << ","<<order[5] << "," 
                <<order[6] << "," <<order[7] << "," 
                <<order[8] <<  ","<<order[9] << "," << endl;
        
    }
    
    csvFile.close();
}
//================================================================================================





// Process CSV file and create PlacedOrder objects
vector<PlacedOrder> parseOrdersFromCSV(const string& filePath)
{
    vector<PlacedOrder> orders;
    ifstream csvFile(filePath);
    
    if (!csvFile.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return orders;
    }
    
    string line;
    bool firstLine = true;
    
    while (getline(csvFile, line)) {
        // Skip header line - first line with column names
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        if (line.empty()) continue;
        
        stringstream ss(line);
        string clientOrderID, instrument, side, quantity, price;
        
       // 1. Check that we actually have 5 separate columns 
       // no 4 commas per line, we cannot figure out which one is missing for same data types
       // only able to disregard such line, not included in execution report
        if (!getline(ss, clientOrderID, ',') ||
            !getline(ss, instrument, ',') ||
            !getline(ss, side, ',') ||
            !getline(ss, quantity, ',') ||
            !getline(ss, price, ',')) {
            cerr << "Warning: Skipping malformed line (missing columns): " << line << endl;
            continue; 
        }
        
        // 2. Trim whitespace from all extracted strings
        string clientOrderIDStr = trim(clientOrderID);
        string instrumentStr = trim(instrument);
        string sideStr = trim(side);
        string quantityStr = trim(quantity);
        string priceStr = trim(price);


                                  
        try {

        
            string curr_time = getCurrentTimeStamp();
            // 2. Safe Conversion
            errorIdentifier = 1;
            int side = stoi(sideStr);
            
            errorIdentifier = 2;
            int quantity = stoi(quantityStr);

            errorIdentifier = 3;
            double price = stod(priceStr);


            // Create PlacedOrder
            PlacedOrder order(clientOrderIDStr, instrumentStr, side, price, quantity); // add to vector of placed orders .
            orders.push_back(order);   // later  we will place in relevant order book at correct side, correct priority row

        } catch (const exception& e) {
            //if any error in string to int/double conversion, I will directly include them in
            // execution report and rejectd execution report
            

            //find the actual column of error/ reason using e.what() and errorIdentifier.
            // directly append to a list in order
            if (errorIdentifier ==1){
                vector<string> entry1 = {"UNEXEC", clientOrderIDStr,instrumentStr, sideStr, priceStr,quantityStr,
                     "Reject",curr_time, priceStr};
                vector<string> entry2 = {"UNEXEC", clientOrderIDStr,instrumentStr, sideStr, priceStr,quantityStr,
                     "Reject","Invalid side",curr_time, priceStr};

                extreme_errors_report.push_back(entry1);
                extreme_errors_rejected_report.push_back(entry2);


            }else if (errorIdentifier ==2){
                vector<string> entry1 = {"UNEXEC", clientOrderIDStr,instrumentStr, sideStr, priceStr,quantityStr,
                     "Reject",curr_time, priceStr};
                vector<string> entry2 = {"UNEXEC", clientOrderIDStr,instrumentStr, sideStr, priceStr,quantityStr,
                     "Reject","Invalid quantity",curr_time, priceStr};

                extreme_errors_report.push_back(entry1);
                extreme_errors_rejected_report.push_back(entry2);

            }else if (errorIdentifier ==3){
                vector<string> entry1 = {"UNEXEC", clientOrderIDStr,instrumentStr, sideStr, priceStr,quantityStr,
                     "Reject",curr_time, priceStr};
                vector<string> entry2 = {"UNEXEC", clientOrderIDStr,instrumentStr, sideStr, priceStr,quantityStr,
                     "Reject","Invalid price",curr_time, priceStr};

                extreme_errors_report.push_back(entry1);
                extreme_errors_rejected_report.push_back(entry2);

            }  
        } 
    }

    
    csvFile.close();
    return orders;
}



//================== start server =================================================================

int start_server() {
    cout << "\n" << string(70, '=') << endl;
    cout << "FLOWER EXCHANGE SERVER - STARTING" << endl;
    cout << string(70, '=') << endl;
    
    // Create exchange application
    ExchangeApplication exchange;
    
    // Option 1: Use GUI to select CSV file
    cout << "Select a CSV file from GUI: " << endl;

    string csvPath;
    cout << "\nOpening file dialog..." << endl;
    csvPath = get_file_path_from_gui();
        
    if (csvPath.empty()) {
        cout << "No file selected. Using default sample file." << endl;
        csvPath = "./reports/order.csv";
    }
    
    cout << "\nLoading orders from: " << csvPath << endl;
    cout << string(70, '-') << endl;
    
    //=========== start measuring time ========================
    // Record start time
    auto start = high_resolution_clock::now();

    // Parse orders from CSV
    vector<PlacedOrder> orders = parseOrdersFromCSV(csvPath);
    
    if (orders.empty()) {
        cerr << "No orders found in CSV file." << endl;
        return 1;
    }
    
    cout << string(70, '-') << endl;
    
    // Process each order through the exchange
    cout << "\nProcessing orders through exchange..." << endl;
    cout << string(70, '-') << endl;
    
    for (auto& order : orders) {
        exchange.processPlacedOrder(order);
    }
    
    cout << "\nAll orders processed!" << endl;
    cout << string(70, '-') << endl;
    
    // Generate execution report
    exchange.generateExecutionReport();
    
    // write to 2 csv files
    exchange.getExecutionReport().displayExecutionReportSummary();


    // append extreme faulty transactions directly to CSV
    // Append multiple orders to report and rejected report files
    appendMultipleOrders_report("./reports/execution_rep.csv", extreme_errors_report);
    appendMultipleOrders_rejected_report("./reports/rejected_execution_rep.csv", extreme_errors_rejected_report);

    
    
    //================stop measuring time========================
    // Record end time and calculate duration
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);  
    
    // Display only milliseconds
    cout << "\nExecution time: " << duration.count() << " milliseconds" << endl;




    //  open output CSV in GUI
    cout << "\nOpening execution report in GUI..." << endl;
    openOutputCSV();
    
    cout << "\nServer operation completed successfully." << endl;
    cout << string(70, '=') << endl;
    
    return 0;
}

