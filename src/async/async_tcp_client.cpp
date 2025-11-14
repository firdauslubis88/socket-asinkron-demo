/**
 * Async TCP Client - Simple Client untuk Async Server
 *
 * Program ini membuat client TCP yang bisa connect ke async_tcp_server:
 * 1. Connect ke async server
 * 2. Kirim pesan
 * 3. Terima echo dari server
 *
 * Catatan:
 * - Client side biasanya tidak perlu async (karena hanya connect ke 1 server)
 * - File ini hampir sama dengan tcp_client.cpp
 * - Dibuat terpisah untuk konsistensi dengan async_tcp_server
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude async_tcp_client.cpp -o async_client.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./async_client.exe
 *
 * Test dengan:
 *   - Jalankan async_tcp_server.exe dulu
 *   - Bisa jalankan multiple client bersamaan!
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
const int SERVER_PORT = 8080;
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

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Async TCP Client - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Connecting to async server..." << std::endl;
    std::cout << "Server: " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Initialize Winsock
    if (!initWinsock()) {
        return 1;
    }

    // 2. Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket created" << std::endl;

    // 3. Set server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << SERVER_IP << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    // 4. Connect to server
    std::cout << "[...] Connecting to async server..." << std::endl;

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "\n[ERROR] Connection failed!" << std::endl;
        std::cerr << "Make sure async_tcp_server.exe is running" << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    std::cout << "[OK] Connected to async server!" << std::endl;

    // 5. Receive welcome message
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "\n[SERVER] " << buffer << std::endl;
    }

    std::cout << "\nYou can now send messages." << std::endl;
    std::cout << "Type 'quit' to disconnect.\n" << std::endl;

    // 6. Main communication loop
    std::string input;
    int messageCount = 0;

    while (true) {
        // Get user input
        std::cout << "You: ";
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        // Add newline
        input += "\n";
        messageCount++;

        // Send to server
        int bytesSent = send(clientSocket, input.c_str(), input.length(), 0);

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "\n[ERROR] Send failed!" << std::endl;
            break;
        }

        // Check for quit
        if (input.find("quit") != std::string::npos) {
            std::cout << "[INFO] Disconnecting..." << std::endl;
            break;
        }

        // Receive response
        memset(buffer, 0, BUFFER_SIZE);
        bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Server: " << buffer;
        } else if (bytesReceived == 0) {
            std::cout << "\n[INFO] Server closed connection" << std::endl;
            break;
        } else {
            std::cerr << "\n[ERROR] Receive failed!" << std::endl;
            break;
        }
    }

    // 7. Cleanup
    std::cout << "\n[INFO] Closing connection..." << std::endl;
    closesocket(clientSocket);
    cleanupWinsock();

    std::cout << "[OK] Disconnected from server" << std::endl;
    std::cout << "Messages sent: " << messageCount << std::endl;
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}
