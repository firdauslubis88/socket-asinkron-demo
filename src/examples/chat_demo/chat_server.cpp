/**
 * Chat Server - Multi-Client Chat Application
 *
 * Program ini membuat chat server yang:
 * 1. Support multiple clients bersamaan (using select())
 * 2. Broadcast messages ke semua clients
 * 3. Track username untuk setiap client
 * 4. Join/leave notifications
 * 5. Support commands (/users, /quit)
 *
 * Ini adalah contoh aplikasi lengkap yang menggabungkan:
 * - Async programming (select())
 * - Message broadcasting
 * - Client management
 * - Text protocol
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude chat_server.cpp -o chat_server.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./chat_server.exe
 *
 * Test dengan:
 *   - Jalankan multiple chat_client.exe
 *   - Semua messages di-broadcast ke semua clients!
 */

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
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

const int PORT = 7777;  // Chat server port
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

/**
 * Client information structure
 */
struct ChatClient {
    SOCKET socket;
    std::string username;
    std::string address;
    time_t joinTime;
    int messageCount;

    ChatClient(SOCKET s, const std::string& addr)
        : socket(s), username(""), address(addr),
          joinTime(time(nullptr)), messageCount(0) {}
};

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
    std::cout << "Winsock initialized successfully" << std::endl;
#endif
    return true;
}

/**
 * Cleanup Winsock (Windows only)
 */
void cleanupWinsock() {
#ifdef _WIN32
    WSACleanup();
    std::cout << "Winsock cleaned up" << std::endl;
#endif
}

/**
 * Broadcast message to all clients (except sender)
 */
void broadcast(const std::vector<ChatClient>& clients, const std::string& message, SOCKET excludeSocket = INVALID_SOCKET) {
    for (const auto& client : clients) {
        if (client.socket != excludeSocket) {
            send(client.socket, message.c_str(), message.length(), 0);
        }
    }
}

/**
 * Get formatted current time
 */
std::string getCurrentTime() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    return std::string(buf);
}

/**
 * Get list of all usernames
 */
std::string getUserList(const std::vector<ChatClient>& clients) {
    std::string list = "Online users (" + std::to_string(clients.size()) + "):\n";
    for (size_t i = 0; i < clients.size(); i++) {
        list += "  " + std::to_string(i + 1) + ". " + clients[i].username + "\n";
    }
    return list;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "      Chat Server - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Port: " << PORT << std::endl;
    std::cout << "Max clients: " << MAX_CLIENTS << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Initialize Winsock
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
    std::cout << "[OK] Chat server listening..." << std::endl;
    std::cout << "\nWaiting for chat clients...\n" << std::endl;

    // Client list
    std::vector<ChatClient> clients;
    char buffer[BUFFER_SIZE];

    // Main select() loop
    while (true) {
        // Prepare fd_set
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(serverSocket, &readFds);
        SOCKET maxFd = serverSocket;

        // Add all client sockets
        for (const auto& client : clients) {
            FD_SET(client.socket, &readFds);
            if (client.socket > maxFd) {
                maxFd = client.socket;
            }
        }

        // Wait for activity
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(maxFd + 1, &readFds, NULL, NULL, &timeout);

        if (activity < 0) {
            std::cerr << "[ERROR] select() failed!" << std::endl;
            break;
        }

        if (activity == 0) {
            continue;  // Timeout
        }

        // Check for new connection
        if (FD_ISSET(serverSocket, &readFds)) {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            SOCKET newClient = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);

            if (newClient != INVALID_SOCKET) {
                if (clients.size() >= MAX_CLIENTS) {
                    std::cout << "[WARNING] Max clients reached. Rejecting connection." << std::endl;
                    std::string msg = "Server full. Try again later.\n";
                    send(newClient, msg.c_str(), msg.length(), 0);
                    closesocket(newClient);
                } else {
                    // Get client address
                    char clientIP[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
                    int clientPort = ntohs(clientAddr.sin_port);
                    std::string clientAddrStr = std::string(clientIP) + ":" + std::to_string(clientPort);

                    // Add to client list (username will be set when they send first message)
                    clients.emplace_back(newClient, clientAddrStr);

                    std::cout << "\n[+] New connection from: " << clientAddrStr << std::endl;
                    std::cout << "    Waiting for username...\n" << std::endl;

                    // Send welcome message
                    std::string welcome = "Welcome to Chat Server! Please enter your username:\n";
                    send(newClient, welcome.c_str(), welcome.length(), 0);
                }
            }
        }

        // Check all client sockets
        for (auto it = clients.begin(); it != clients.end(); ) {
            if (FD_ISSET(it->socket, &readFds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytesReceived = recv(it->socket, buffer, BUFFER_SIZE - 1, 0);

                if (bytesReceived > 0) {
                    buffer[bytesReceived] = '\0';

                    // Remove trailing newline
                    std::string message(buffer);
                    if (!message.empty() && message[message.length() - 1] == '\n') {
                        message = message.substr(0, message.length() - 1);
                    }

                    // If username not set, this is the username
                    if (it->username.empty()) {
                        it->username = message;
                        std::cout << "[+] User '" << it->username << "' joined the chat" << std::endl;
                        std::cout << "    Total users: " << clients.size() << "\n" << std::endl;

                        // Notify all other clients
                        std::string joinMsg = "[SERVER] " + it->username + " joined the chat!\n";
                        broadcast(clients, joinMsg, it->socket);

                        // Send confirmation to new user
                        std::string confirm = "Welcome, " + it->username + "! Type /users to see who's online.\n";
                        send(it->socket, confirm.c_str(), confirm.length(), 0);

                        ++it;
                        continue;
                    }

                    it->messageCount++;

                    // Handle commands
                    if (message == "/users") {
                        std::string userList = getUserList(clients);
                        send(it->socket, userList.c_str(), userList.length(), 0);
                        ++it;
                        continue;
                    } else if (message == "/quit") {
                        std::string byeMsg = "Goodbye, " + it->username + "!\n";
                        send(it->socket, byeMsg.c_str(), byeMsg.length(), 0);

                        std::cout << "[-] User '" << it->username << "' left the chat" << std::endl;
                        std::string leaveMsg = "[SERVER] " + it->username + " left the chat.\n";
                        broadcast(clients, leaveMsg, it->socket);

                        closesocket(it->socket);
                        it = clients.erase(it);
                        continue;
                    }

                    // Broadcast message to all clients
                    std::cout << "[" << getCurrentTime() << "] <" << it->username << "> " << message << std::endl;

                    std::string broadcastMsg = "<" + it->username + "> " + message + "\n";
                    broadcast(clients, broadcastMsg, it->socket);

                    // Send confirmation to sender
                    std::string echo = "[You] " + message + "\n";
                    send(it->socket, echo.c_str(), echo.length(), 0);

                    ++it;
                } else {
                    // Client disconnected
                    if (!it->username.empty()) {
                        std::cout << "\n[-] User '" << it->username << "' disconnected" << std::endl;
                        std::cout << "    Messages sent: " << it->messageCount << "\n" << std::endl;

                        std::string leaveMsg = "[SERVER] " + it->username + " disconnected.\n";
                        broadcast(clients, leaveMsg, it->socket);
                    }

                    closesocket(it->socket);
                    it = clients.erase(it);
                }
            } else {
                ++it;
            }
        }
    }

    // Cleanup
    for (auto& client : clients) {
        closesocket(client.socket);
    }
    closesocket(serverSocket);
    cleanupWinsock();

    return 0;
}
