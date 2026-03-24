CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -Wno-multichar -Wno-overflow
LIBS     = -lQt5Widgets -lQt5Core -lQt5Gui

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Target
TARGET = $(BIN_DIR)/server

# Sources & Objects
SRCS = main.cpp \
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

OBJS = $(patsubst %, $(OBJ_DIR)/%, $(notdir $(SRCS:.cpp=.o)))

# Rules
all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean