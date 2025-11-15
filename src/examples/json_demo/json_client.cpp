/**
 * JSON Client Demo - Simple TCP Client dengan JSON Messages
 *
 * Program ini mendemonstrasikan:
 * 1. Mengirim messages dalam format JSON
 * 2. Menerima dan parse JSON responses
 * 3. Struktur message protocol menggunakan JSON
 *
 * Message format:
 * {
 *   "type": "message" | "command" | "response",
 *   "content": "message text",
 *   "timestamp": 1234567890
 * }
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -I../../utils json_client.cpp -o json_client.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./json_client.exe
 */

#include <iostream>
#include <string>
#include <cstring>

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

// JSON Helper (simplified version)
#include "../../utils/json_helper.hpp"

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 9999;  // JSON demo server port
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

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  JSON Client Demo - Simple Protocol  " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Server: " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    if (!initWinsock()) {
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket created" << std::endl;

    // Connect to server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    std::cout << "[...] Connecting to server..." << std::endl;
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "\n[ERROR] Connection failed!" << std::endl;
        std::cerr << "Make sure json_server is running!" << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Connected!\n" << std::endl;

    // Main loop
    char buffer[BUFFER_SIZE];
    std::string input;

    std::cout << "Commands:" << std::endl;
    std::cout << "  <message>  - Send message as JSON" << std::endl;
    std::cout << "  /echo      - Send echo command" << std::endl;
    std::cout << "  /time      - Request server time" << std::endl;
    std::cout << "  /quit      - Disconnect\n" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        // Prepare JSON message
        std::string jsonMessage;

        if (input == "/quit") {
            jsonMessage = createCommandJsonMessage("quit");
            std::cout << "\n[INFO] Disconnecting...\n" << std::endl;
        } else if (input == "/echo") {
            jsonMessage = createCommandJsonMessage("echo", "Hello from client!");
        } else if (input == "/time") {
            jsonMessage = createCommandJsonMessage("time");
        } else {
            // Regular message
            jsonMessage = createSimpleJsonMessage("message", input);
        }

        // Display what we're sending
        std::cout << "[SEND] " << jsonMessage << std::endl;

        // Send JSON message (add newline as delimiter)
        jsonMessage += "\n";
        int bytesSent = send(clientSocket, jsonMessage.c_str(), jsonMessage.length(), 0);

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "[ERROR] Send failed!" << std::endl;
            break;
        }

        // Quit immediately
        if (input == "/quit") {
            break;
        }

        // Receive response
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Remove trailing newline
            std::string response(buffer);
            if (!response.empty() && response[response.length() - 1] == '\n') {
                response = response.substr(0, response.length() - 1);
            }

            std::cout << "[RECV] " << response << std::endl;

            // Parse JSON response (simple)
            if (jsonContainsKey(response, "type")) {
                std::string type = extractJsonValue(response, "type");
                std::string content = extractJsonValue(response, "content");

                std::cout << "[PARSED] Type: " << type
                          << ", Content: " << content << "\n" << std::endl;
            }

        } else if (bytesReceived == 0) {
            std::cout << "\n[INFO] Server closed connection" << std::endl;
            break;
        } else {
            std::cerr << "\n[ERROR] Receive failed!" << std::endl;
            break;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    cleanupWinsock();

    std::cout << "\n[OK] Disconnected" << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
