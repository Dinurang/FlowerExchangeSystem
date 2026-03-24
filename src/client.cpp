// #include "socket.h"
// #include "gui.h" 
// #include <iostream>
// #include <fstream>
// #include <string>


// void send_csv_to_server(const std::string& filepath) {
//     int sock = 0;
//     struct sockaddr_in serv_addr;
    
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         std::cerr << "Socket creation error" << std::endl;
//         return;
//     }

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // Convert IPv4 address from text to binary
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         return;
//     }

//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//         std::cerr << "Connection Failed" << std::endl;
//         return;
//     }

//     std::ifstream file(filepath);
//     if (!file.is_open()) {
//         std::cerr << "Could not open file: " << filepath << std::endl;
//         return;
//     }

//     std::string line;
//     while (std::getline(file, line)) {
//         line += "\n"; // Ensure server knows where line ends
//         send(sock, line.c_str(), line.length(), 0);
//     }

//     std::cout << "CSV data sent successfully." << std::endl;
//     close(sock);
// }





// int main() {
//     std::cout << "Waiting for file selection via GUI..." << std::endl;
    
//     // Call the function from gui.cpp
//     std::string selected_file = get_file_path_from_gui();

//     if (selected_file.empty()) {
//         std::cerr << "No file selected or dialog canceled. Exiting." << std::endl;
//         return 1;
//     }

//     std::cout << "Selected file: " << selected_file << std::endl;
    
//     // Send the chosen file
//     send_csv_to_server(selected_file);
    
//     return 0;
// }