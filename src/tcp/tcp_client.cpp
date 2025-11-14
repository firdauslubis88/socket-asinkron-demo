/**
 * TCP Client - Echo Client Sederhana
 *
 * Program ini membuat client TCP yang:
 * 1. Connect ke server di localhost:8080
 * 2. Kirim pesan yang diinput user
 * 3. Terima response dari server (echo)
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude tcp_client.cpp -o tcp_client.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./tcp_client.exe
 *
 * Catatan:
 *   - Server harus running dulu (tcp_server.exe)
 *   - Ketik 'quit' untuk disconnect
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

const char* SERVER_IP = "127.0.0.1";  // localhost
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
    std::cout << "    TCP Echo Client - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Connecting to: " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Initialize Winsock (Windows)
    if (!initWinsock()) {
        return 1;
    }

    // 2. Buat socket
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

    // Convert IP address dari string ke binary
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << SERVER_IP << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    // 4. Connect ke server
    std::cout << "[...] Connecting to server..." << std::endl;

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "\n[ERROR] Connection failed!" << std::endl;
        std::cerr << "Pastikan server sudah running (tcp_server.exe)" << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    std::cout << "[OK] Connected to server!" << std::endl;
    std::cout << "\nYou can now send messages to the server." << std::endl;
    std::cout << "Type 'quit' to disconnect.\n" << std::endl;

    // 5. Main loop - kirim dan terima pesan
    char buffer[BUFFER_SIZE];
    std::string input;

    while (true) {
        // Prompt user untuk input
        std::cout << "You: ";
        std::getline(std::cin, input);

        // Check jika input kosong
        if (input.empty()) {
            continue;
        }

        // Tambahkan newline untuk readability di server
        input += "\n";

        // Kirim pesan ke server
        int bytesSent = send(clientSocket, input.c_str(), input.length(), 0);

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "\n[ERROR] Send failed!" << std::endl;
            break;
        }

        // Check jika user mau quit
        if (input.find("quit") != std::string::npos) {
            std::cout << "[INFO] Disconnecting..." << std::endl;
            break;
        }

        // Terima response dari server
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Server: " << buffer << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "\n[INFO] Server closed connection" << std::endl;
            break;
        } else {
            std::cerr << "\n[ERROR] Receive failed!" << std::endl;
            break;
        }
    }

    // 6. Cleanup
    std::cout << "\n[INFO] Closing connection..." << std::endl;
    closesocket(clientSocket);
    cleanupWinsock();

    std::cout << "[OK] Connection closed" << std::endl;
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}
