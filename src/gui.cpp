#include "gui.h"
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;



// Open both execution and rejected execution CSVs in a text viewer
void openOutputCSV()
{
    string execPath = "./reports/execution_rep.csv";
    string rejectedPath = "./reports/rejected_execution_rep.csv";
    
    // Combine both reports with headers
    string command = "(echo '====== EXECUTED ORDERS ======'; cat " 
    + execPath + "; echo ''; echo '====== REJECTED ORDERS ======'; cat " 
    + rejectedPath + ") | zenity --text-info --title=\"Execution Report\" --width=1500 --height=1000 2>/dev/null &";
    
    if (system(command.c_str()) != 0) {
        cout << "Note: Install 'zenity' for GUI display." << endl;
        cout << "Execution Report: " << execPath << endl;
        cout << "Rejected Report: " << rejectedPath << endl;
        cout << "View with: cat " << execPath << " && cat " << rejectedPath << endl;
    }
}



std::string get_file_path_from_gui() {
    std::string filepath = "";
    char buffer[1024];
    
    // Command to open a GTK file dialog filtering for .csv files
    
    const char* command = "zenity --file-selection --title=\"Select Order CSV File\"  --width=1800 --height=1000  --file-filter=\"*.csv\" 2>/dev/null";
    
    // Open a pipe to read the output of the zenity command
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        std::cerr << "Failed to open GUI file dialog." << std::endl;
        return "";
    }
    
    // Read the selected file path from the dialog
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        filepath = buffer;
        
        // Remove the trailing newline character that fgets/zenity adds
        size_t newline_pos = filepath.find_last_not_of(" \n\r\t");
        if (newline_pos != std::string::npos) {
            filepath.erase(newline_pos + 1);
        }
    }
    
    pclose(pipe);
    return filepath;
}