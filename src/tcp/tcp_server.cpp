/**
 * TCP Server - Echo Server Sederhana
 *
 * Program ini membuat server TCP yang:
 * 1. Listen pada port 8080
 * 2. Menerima koneksi dari client
 * 3. Menerima pesan dari client
 * 4. Mengirim kembali pesan yang sama (echo)
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude tcp_server.cpp -o tcp_server.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./tcp_server.exe
 *
 * Test dengan:
 *   - tcp_client.exe (di terminal lain)
 *   - telnet localhost 8080
 *   - nc localhost 8080
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

const int PORT = 8080;
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
 * Handle client connection
 * Menerima pesan dan echo kembali
 */
void handleClient(SOCKET clientSocket, const std::string& clientAddr) {
    char buffer[BUFFER_SIZE];

    std::cout << "\n[+] Client connected from: " << clientAddr << std::endl;
    std::cout << "    Waiting for messages (type 'quit' to disconnect)...\n" << std::endl;

    while (true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Terima data dari client
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived > 0) {
            // Null-terminate string (PENTING!)
            buffer[bytesReceived] = '\0';

            // Tampilkan pesan yang diterima
            std::cout << "[RECV] " << buffer;

            // Check jika client kirim "quit"
            std::string message(buffer);
            if (message.find("quit") != std::string::npos) {
                std::cout << "[INFO] Client requested disconnect" << std::endl;
                break;
            }

            // Echo kembali ke client
            std::string response = "Echo: " + message;
            int bytesSent = send(clientSocket, response.c_str(), response.length(), 0);

            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "[ERROR] Send failed!" << std::endl;
                break;
            }

            std::cout << "[SEND] " << response;

        } else if (bytesReceived == 0) {
            // Connection closed by client
            std::cout << "[INFO] Client disconnected gracefully" << std::endl;
            break;
        } else {
            // Error occurred
            std::cerr << "[ERROR] recv() failed" << std::endl;
            break;
        }
    }

    // Close client socket
    closesocket(clientSocket);
    std::cout << "[-] Client connection closed\n" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "    TCP Echo Server - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Port: " << PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Initialize Winsock (Windows)
    if (!initWinsock()) {
        return 1;
    }

    // 2. Buat socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket created successfully" << std::endl;

    // 3. Set socket options (SO_REUSEADDR)
    char opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Warning: setsockopt failed (non-critical)" << std::endl;
    }

    // 4. Bind socket ke address dan port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed! Port mungkin sudah digunakan." << std::endl;
        std::cerr << "Coba gunakan port lain atau tunggu beberapa saat." << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket bound to port " << PORT << std::endl;

    // 5. Listen untuk incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Server listening..." << std::endl;
    std::cout << "\nWaiting for client connections...\n" << std::endl;

    // 6. Accept dan handle clients (loop forever)
    while (true) {
        // Accept client connection
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed!" << std::endl;
            continue;  // Try again
        }

        // Get client IP address
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);

        std::string clientAddrStr = std::string(clientIP) + ":" + std::to_string(clientPort);

        // Handle client (blocking - server will handle one client at a time)
        handleClient(clientSocket, clientAddrStr);
    }

    // 7. Cleanup (unreachable in this version, but good practice)
    closesocket(serverSocket);
    cleanupWinsock();

    return 0;
}
