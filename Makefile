# Makefile untuk Socket Asinkron Demo
# Author: Demo untuk mahasiswa Teknik Elektro UI

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
LDFLAGS = -pthread

# Directories
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

# Ensure bin directory exists
$(shell mkdir -p $(BIN_DIR))

# TCP Examples
tcp_server: $(SRC_DIR)/tcp/tcp_server.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ TCP Server compiled successfully"

tcp_client: $(SRC_DIR)/tcp/tcp_client.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ TCP Client compiled successfully"

# UDP Examples
udp_server: $(SRC_DIR)/udp/udp_server.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ UDP Server compiled successfully"

udp_client: $(SRC_DIR)/udp/udp_client.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ UDP Client compiled successfully"

# Async Examples
async_server: $(SRC_DIR)/async/async_tcp_server.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ Async Server compiled successfully"

async_client: $(SRC_DIR)/async/async_tcp_client.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ Async Client compiled successfully"

# Chat Demo Example (akan menggunakan utils dan include)
chat_server: $(SRC_DIR)/examples/chat_demo/chat_server.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ Chat Server compiled successfully"

chat_client: $(SRC_DIR)/examples/chat_demo/chat_client.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)
	@echo "✓ Chat Client compiled successfully"

# Build targets
.PHONY: all tcp udp async chat clean help

all: tcp udp async
	@echo ""
	@echo "================================"
	@echo "All examples compiled successfully!"
	@echo "Run programs from bin/ directory"
	@echo "================================"

tcp: tcp_server tcp_client
	@echo "✓ TCP examples ready"

udp: udp_server udp_client
	@echo "✓ UDP examples ready"

async: async_server async_client
	@echo "✓ Async examples ready"

chat: chat_server chat_client
	@echo "✓ Chat demo ready"

# Clean build artifacts
clean:
	rm -f $(BIN_DIR)/*
	rm -rf $(BUILD_DIR)/*
	@echo "✓ Cleaned build files"

# Help command
help:
	@echo "Socket Asinkron Demo - Build Commands"
	@echo "======================================"
	@echo ""
	@echo "Available targets:"
	@echo "  make all          - Build all examples (tcp, udp, async)"
	@echo "  make tcp          - Build TCP server and client"
	@echo "  make udp          - Build UDP server and client"
	@echo "  make async        - Build async server and client"
	@echo "  make chat         - Build chat demo"
	@echo ""
	@echo "Individual targets:"
	@echo "  make tcp_server   - Build TCP server only"
	@echo "  make tcp_client   - Build TCP client only"
	@echo "  make udp_server   - Build UDP server only"
	@echo "  make udp_client   - Build UDP client only"
	@echo ""
	@echo "Utility targets:"
	@echo "  make clean        - Remove all compiled binaries"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make tcp          # Build TCP examples"
	@echo "  ./bin/tcp_server  # Run TCP server"
	@echo ""
