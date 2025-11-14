/**
 * Chat Client - Multi-User Chat Application Client
 *
 * Program ini membuat chat client yang:
 * 1. Connect ke chat server
 * 2. Input username
 * 3. Kirim dan terima messages
 * 4. Support commands (/users, /quit)
 * 5. Display messages dari users lain
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude chat_client.cpp -o chat_client.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./chat_client.exe
 *
 * Commands:
 *   /users - List all online users
 *   /quit  - Disconnect from chat
 *
 * Test:
 *   - Jalankan chat_server.exe dulu
 *   - Jalankan multiple chat_client.exe
 *   - Chat dengan users lain!
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

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 7777;  // Chat server port
const int BUFFER_SIZE = 1024;

/**
 * Initialize Winsock (Windows only)
 */
bool initWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }
#endif
    return true;
}

/**
 * Cleanup Winsock (Windows only)
 */
void cleanupWinsock() {
#ifdef _WIN32
    WSACleanup();
#endif
}

/**
 * Display chat UI header
 */
void displayHeader(const std::string& username) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "       Chat Room - " << username << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Commands: /users, /quit" << std::endl;
    std::cout << "========================================\n" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "      Chat Client - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Server: " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Initialize Winsock
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

    // Set server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << SERVER_IP << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    // Connect to server
    std::cout << "[...] Connecting to chat server..." << std::endl;

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "\n[ERROR] Connection failed!" << std::endl;
        std::cerr << "Make sure chat_server.exe is running on " << SERVER_IP << ":" << SERVER_PORT << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    std::cout << "[OK] Connected to chat server!\n" << std::endl;

    // Receive welcome message
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }

    // Get username
    std::string username;
    std::getline(std::cin, username);

    // Send username to server
    username += "\n";
    send(clientSocket, username.c_str(), username.length(), 0);

    // Remove newline from username for display
    if (!username.empty() && username[username.length() - 1] == '\n') {
        username = username.substr(0, username.length() - 1);
    }

    // Receive confirmation
    memset(buffer, 0, BUFFER_SIZE);
    bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }

    // Display chat UI
    displayHeader(username);

    // Main chat loop
    std::string input;
    bool running = true;

    while (running) {
        // Get user input
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        // Send message
        std::string message = input + "\n";
        int bytesSent = send(clientSocket, message.c_str(), message.length(), 0);

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "\n[ERROR] Send failed!" << std::endl;
            break;
        }

        // Check for /quit command
        if (input == "/quit") {
            running = false;
            // Receive goodbye message
            memset(buffer, 0, BUFFER_SIZE);
            bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                std::cout << buffer;
            }
            break;
        }

        // Receive response
        memset(buffer, 0, BUFFER_SIZE);
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Display response
            std::cout << buffer;

            // If response is multiline (like /users), don't show prompt yet
            if (input == "/users") {
                std::cout << std::endl;
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
    std::cout << "\n[INFO] Disconnecting from chat server..." << std::endl;
    closesocket(clientSocket);
    cleanupWinsock();

    std::cout << "[OK] Disconnected" << std::endl;
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}
