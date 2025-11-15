/**
 * JSON Server Demo - Simple TCP Server dengan JSON Protocol
 *
 * Program ini mendemonstrasikan:
 * 1. Menerima JSON messages dari client
 * 2. Parse JSON dan handle different message types
 * 3. Respond dengan JSON format
 *
 * Message types yang di-handle:
 * - "message": Regular text message
 * - "command": Server commands (echo, time, quit)
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -I../../utils json_server.cpp -o json_server.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./json_server.exe
 */

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>

// Platform-specific includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

// JSON Helper
#include "../../utils/json_helper.hpp"

const int PORT = 9999;
const int BUFFER_SIZE = 1024;

bool initWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return false;
    }
#endif
    return true;
}

void cleanupWinsock() {
#ifdef _WIN32
    WSACleanup();
#endif
}

/**
 * Get current time as string
 */
std::string getCurrentTimeString() {
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

/**
 * Handle client connection
 */
void handleClient(SOCKET clientSocket, const std::string& clientAddr) {
    std::cout << "\n[+] Client connected: " << clientAddr << std::endl;
    std::cout << "    Waiting for JSON messages...\n" << std::endl;

    char buffer[BUFFER_SIZE];
    bool running = true;

    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Remove trailing newline
            std::string jsonMessage(buffer);
            if (!jsonMessage.empty() && jsonMessage[jsonMessage.length() - 1] == '\n') {
                jsonMessage = jsonMessage.substr(0, jsonMessage.length() - 1);
            }

            std::cout << "[RECV] " << jsonMessage << std::endl;

            // Parse JSON
            if (!jsonContainsKey(jsonMessage, "type")) {
                std::cerr << "[ERROR] Invalid JSON: missing 'type' field" << std::endl;
                continue;
            }

            std::string messageType = extractJsonValue(jsonMessage, "type");
            std::string responseJson;

            // Handle different message types
            if (messageType == "message") {
                // Regular message - echo back
                std::string content = extractJsonValue(jsonMessage, "content");
                std::cout << "[MESSAGE] Client says: " << content << std::endl;

                responseJson = createSimpleJsonMessage("response", "Message received: " + content);

            } else if (messageType == "command") {
                // Command message
                std::string command = extractJsonValue(jsonMessage, "command");
                std::cout << "[COMMAND] " << command << std::endl;

                if (command == "quit") {
                    responseJson = createSimpleJsonMessage("response", "Goodbye!");
                    running = false;

                } else if (command == "echo") {
                    std::string args = extractJsonValue(jsonMessage, "args");
                    responseJson = createSimpleJsonMessage("response", "Echo: " + args);

                } else if (command == "time") {
                    std::string timeStr = getCurrentTimeString();
                    responseJson = createSimpleJsonMessage("response", "Server time: " + timeStr);

                } else {
                    responseJson = createSimpleJsonMessage("error", "Unknown command: " + command);
                }

            } else {
                // Unknown message type
                responseJson = createSimpleJsonMessage("error", "Unknown message type: " + messageType);
            }

            // Send response
            std::cout << "[SEND] " << responseJson << std::endl;
            responseJson += "\n";
            send(clientSocket, responseJson.c_str(), responseJson.length(), 0);

        } else if (bytesReceived == 0) {
            std::cout << "\n[-] Client disconnected: " << clientAddr << std::endl;
            break;
        } else {
            std::cerr << "\n[ERROR] Receive failed!" << std::endl;
            break;
        }
    }

    closesocket(clientSocket);
    std::cout << "[-] Connection closed: " << clientAddr << "\n" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  JSON Server Demo - Simple Protocol  " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Port: " << PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    if (!initWinsock()) {
        return 1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket created" << std::endl;

    // Set socket options
    char opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket bound to port " << PORT << std::endl;

    // Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Server listening..." << std::endl;
    std::cout << "\nWaiting for JSON clients...\n" << std::endl;

    // Accept loop (simple, one client at a time)
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "[ERROR] Accept failed!" << std::endl;
            continue;
        }

        // Get client address
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);
        std::string clientAddrStr = std::string(clientIP) + ":" + std::to_string(clientPort);

        // Handle client (blocking - one at a time for simplicity)
        handleClient(clientSocket, clientAddrStr);
    }

    // Cleanup
    closesocket(serverSocket);
    cleanupWinsock();

    return 0;
}
