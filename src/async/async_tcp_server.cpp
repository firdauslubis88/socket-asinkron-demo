/**
 * Async TCP Server - Multiple Clients dengan select()
 *
 * Program ini membuat server TCP yang bisa handle BANYAK clients bersamaan:
 * 1. Menggunakan select() untuk monitor multiple sockets
 * 2. Non-blocking - tidak stuck di satu client
 * 3. Echo pesan dari clients
 *
 * Perbedaan dengan tcp_server.cpp:
 * - Bisa handle multiple clients simultaneously
 * - Menggunakan select() untuk async I/O
 * - Tidak blocking saat wait for clients
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude async_tcp_server.cpp -o async_server.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./async_server.exe
 *
 * Test dengan multiple clients:
 *   - Jalankan tcp_client.exe di beberapa terminal berbeda
 *   - Server bisa handle semua clients bersamaan!
 */

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

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

const int PORT = 8080;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

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
 * Client info structure
 */
struct ClientInfo {
    SOCKET socket;
    std::string address;
    int messageCount;

    ClientInfo(SOCKET s, const std::string& addr)
        : socket(s), address(addr), messageCount(0) {}
};

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Async TCP Server - select() Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Port: " << PORT << std::endl;
    std::cout << "Max clients: " << MAX_CLIENTS << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Initialize Winsock
    if (!initWinsock()) {
        return 1;
    }

    // 2. Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket created" << std::endl;

    // 3. Set socket options
    char opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 4. Bind
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

    // 5. Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Server listening..." << std::endl;
    std::cout << "\nWaiting for clients (can handle multiple simultaneously)...\n" << std::endl;

    // 6. Client list
    std::vector<ClientInfo> clients;
    char buffer[BUFFER_SIZE];

    // 7. Main select() loop
    while (true) {
        // Prepare fd_set untuk select()
        fd_set readFds;
        FD_ZERO(&readFds);

        // Add server socket
        FD_SET(serverSocket, &readFds);
        SOCKET maxFd = serverSocket;

        // Add all client sockets
        for (const auto& client : clients) {
            FD_SET(client.socket, &readFds);
            if (client.socket > maxFd) {
                maxFd = client.socket;
            }
        }

        // Wait for activity (timeout: 1 second)
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(maxFd + 1, &readFds, NULL, NULL, &timeout);

        if (activity < 0) {
            std::cerr << "[ERROR] select() failed!" << std::endl;
            break;
        }

        if (activity == 0) {
            // Timeout - no activity, loop continues
            continue;
        }

        // Check if server socket has activity (new connection)
        if (FD_ISSET(serverSocket, &readFds)) {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);

            SOCKET newClient = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);

            if (newClient != INVALID_SOCKET) {
                // Check if we can accept more clients
                if (clients.size() >= MAX_CLIENTS) {
                    std::cout << "[WARNING] Max clients reached. Rejecting connection." << std::endl;
                    closesocket(newClient);
                } else {
                    // Get client address
                    char clientIP[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
                    int clientPort = ntohs(clientAddr.sin_port);

                    std::string clientAddrStr = std::string(clientIP) + ":" + std::to_string(clientPort);

                    // Add to client list
                    clients.emplace_back(newClient, clientAddrStr);

                    std::cout << "\n[+] New client connected: " << clientAddrStr << std::endl;
                    std::cout << "    Total clients: " << clients.size() << "\n" << std::endl;

                    // Send welcome message
                    std::string welcome = "Welcome to Async TCP Server! You are client #" +
                                        std::to_string(clients.size()) + "\n";
                    send(newClient, welcome.c_str(), welcome.length(), 0);
                }
            }
        }

        // Check all client sockets for activity
        for (auto it = clients.begin(); it != clients.end(); ) {
            if (FD_ISSET(it->socket, &readFds)) {
                // This client has data to read
                memset(buffer, 0, BUFFER_SIZE);

                int bytesReceived = recv(it->socket, buffer, BUFFER_SIZE - 1, 0);

                if (bytesReceived > 0) {
                    // Data received
                    buffer[bytesReceived] = '\0';
                    it->messageCount++;

                    std::cout << "[" << it->address << "] ";
                    std::cout << "Message #" << it->messageCount << ": " << buffer;

                    // Echo back with prefix
                    std::string response = "Echo [" + std::to_string(it->messageCount) + "]: " +
                                         std::string(buffer);
                    send(it->socket, response.c_str(), response.length(), 0);

                    ++it;

                } else {
                    // Client disconnected or error
                    std::cout << "\n[-] Client disconnected: " << it->address << std::endl;
                    std::cout << "    Messages received: " << it->messageCount << std::endl;
                    std::cout << "    Remaining clients: " << (clients.size() - 1) << "\n" << std::endl;

                    closesocket(it->socket);
                    it = clients.erase(it);
                }
            } else {
                ++it;
            }
        }
    }

    // 8. Cleanup
    for (auto& client : clients) {
        closesocket(client.socket);
    }
    closesocket(serverSocket);
    cleanupWinsock();

    return 0;
}
