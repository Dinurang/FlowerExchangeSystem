# Compiler and Flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -Wno-multichar -Wno-overflow
LDFLAGS  = 
LIBS     = -lQt5Widgets -lQt5Core -lQt5Gui

# Directory names
SRC_DIR  = src
OBJ_DIR  = obj
BIN_DIR  = bin

# Target executable names
SERVER_TARGET = $(BIN_DIR)/server
CLIENT_TARGET = $(BIN_DIR)/client

# 1. Define sources for the SERVER (Exchange Application)
SERVER_SRCS = main.cpp \
              $(SRC_DIR)/server.cpp \
              $(SRC_DIR)/orderbook.cpp \
              $(SRC_DIR)/exchange_application.cpp \
              $(SRC_DIR)/order.cpp \
              $(SRC_DIR)/bookside.cpp \
              $(SRC_DIR)/buyside.cpp \
              $(SRC_DIR)/sellside.cpp \
              $(SRC_DIR)/executed_order.cpp \
              $(SRC_DIR)/execution_report.cpp \
              $(SRC_DIR)/placed_order.cpp \
              $(SRC_DIR)/gui.cpp

# 2. Define sources for the CLIENT
CLIENT_SRCS = $(SRC_DIR)/client.cpp \
              $(SRC_DIR)/gui.cpp

# Transform sources to objects
SERVER_OBJS = $(patsubst %, $(OBJ_DIR)/%, $(notdir $(SERVER_SRCS:.cpp=.o)))
CLIENT_OBJS = $(patsubst %, $(OBJ_DIR)/%, $(notdir $(CLIENT_SRCS:.cpp=.o)))

# Default rule: build server only (client.cpp is incomplete)
all: $(SERVER_TARGET)

# Link the Server executable
$(SERVER_TARGET): $(SERVER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(SERVER_OBJS) -o $(SERVER_TARGET) $(LDFLAGS) $(LIBS)

# Link the Client executable
$(CLIENT_TARGET): $(CLIENT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CLIENT_OBJS) -o $(CLIENT_TARGET) $(LDFLAGS) $(LIBS)

# Rule for compiling src/*.cpp files into obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule specifically for main.cpp (since it's in the root)
$(OBJ_DIR)/main.o: main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean