#include <iostream>
#include "gui.h"
#include "server.h"
#include "client.h"
using namespace std;




int main() {
    cout << "Initializing Exchange System..." << endl;
    
    start_server(); 
    
    return 0;
}