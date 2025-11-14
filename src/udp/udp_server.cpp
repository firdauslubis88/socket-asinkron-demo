/**
 * UDP Server - Echo Server Sederhana (Connectionless)
 *
 * Program ini membuat server UDP yang:
 * 1. Bind ke port 9000
 * 2. Terima datagram dari client (tidak perlu connection!)
 * 3. Kirim kembali datagram yang sama (echo)
 *
 * Perbedaan dengan TCP:
 * - Tidak ada listen() dan accept()
 * - Gunakan recvfrom() dan sendto() (bukan recv/send)
 * - Setiap datagram independent
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude udp_server.cpp -o udp_server.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./udp_server.exe
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

const int PORT = 9000;  // UDP port (berbeda dari TCP port)
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

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "    UDP Echo Server - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Protocol: UDP (Connectionless)" << std::endl;
    std::cout << "Port: " << PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Initialize Winsock (Windows)
    if (!initWinsock()) {
        return 1;
    }

    // 2. Buat UDP socket (SOCK_DGRAM untuk UDP)
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] UDP socket created successfully" << std::endl;

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
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] Socket bound to port " << PORT << std::endl;

    // 5. Ready to receive datagrams (TIDAK perlu listen/accept!)
    std::cout << "[OK] Server ready to receive datagrams..." << std::endl;
    std::cout << "\nWaiting for UDP datagrams...\n" << std::endl;

    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int packetCount = 0;

    // 6. Loop forever - terima dan echo datagrams
    while (true) {
        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);
        memset(&clientAddr, 0, sizeof(clientAddr));
        clientAddrLen = sizeof(clientAddr);

        // Terima datagram dari client (blocking)
        // recvfrom() juga sekaligus dapatkan address client
        int bytesReceived = recvfrom(
            serverSocket,
            buffer,
            BUFFER_SIZE - 1,
            0,
            (sockaddr*)&clientAddr,
            &clientAddrLen
        );

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "[ERROR] recvfrom() failed!" << std::endl;
            continue;
        }

        // Null-terminate buffer
        buffer[bytesReceived] = '\0';

        // Get client address info
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);

        packetCount++;

        // Display received datagram
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Packet #" << packetCount << std::endl;
        std::cout << "From: " << clientIP << ":" << clientPort << std::endl;
        std::cout << "Size: " << bytesReceived << " bytes" << std::endl;
        std::cout << "Data: " << buffer;
        std::cout << "-----------------------------------" << std::endl;

        // Echo kembali ke client
        std::string response = "UDP Echo: " + std::string(buffer);

        int bytesSent = sendto(
            serverSocket,
            response.c_str(),
            response.length(),
            0,
            (sockaddr*)&clientAddr,
            clientAddrLen
        );

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "[ERROR] sendto() failed!" << std::endl;
        } else {
            std::cout << "[SENT] Echoed " << bytesSent << " bytes back to client\n" << std::endl;
        }
    }

    // 7. Cleanup (unreachable in this version)
    closesocket(serverSocket);
    cleanupWinsock();

    return 0;
}
