#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <vector>

#define PORT 8080
#define BUFFER_SIZE 1024

// Utility to handle socket setup
int create_client_socket(const char* ip);
int create_server_socket();

#endif