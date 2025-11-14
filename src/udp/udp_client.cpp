/**
 * UDP Client - Echo Client Sederhana (Connectionless)
 *
 * Program ini membuat client UDP yang:
 * 1. Buat UDP socket
 * 2. Kirim datagram ke server (TIDAK perlu connect!)
 * 3. Terima response dari server
 *
 * Perbedaan dengan TCP:
 * - Tidak ada connect()
 * - Gunakan sendto() dan recvfrom()
 * - Setiap datagram independent
 *
 * Cara compile (Windows):
 *   g++ -std=c++11 -Iinclude udp_client.cpp -o udp_client.exe -lws2_32
 *
 * Cara menjalankan:
 *   ./udp_client.exe
 *
 * Catatan:
 *   - Server harus running dulu (udp_server.exe)
 *   - Ketik 'quit' untuk exit
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
const int SERVER_PORT = 9000;          // UDP server port
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
    std::cout << "    UDP Echo Client - Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Protocol: UDP (Connectionless)" << std::endl;
    std::cout << "Server: " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Initialize Winsock (Windows)
    if (!initWinsock()) {
        return 1;
    }

    // 2. Buat UDP socket (SOCK_DGRAM untuk UDP)
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        cleanupWinsock();
        return 1;
    }
    std::cout << "[OK] UDP socket created" << std::endl;

    // 3. Set server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << SERVER_IP << std::endl;
        closesocket(clientSocket);
        cleanupWinsock();
        return 1;
    }

    std::cout << "[OK] Ready to send datagrams" << std::endl;
    std::cout << "\nYou can now send messages to the server." << std::endl;
    std::cout << "Type 'quit' to exit.\n" << std::endl;

    // 4. Main loop - kirim dan terima datagrams
    char buffer[BUFFER_SIZE];
    std::string input;
    sockaddr_in responseAddr;
    socklen_t responseAddrLen = sizeof(responseAddr);
    int messageCount = 0;

    while (true) {
        // Prompt user untuk input
        std::cout << "You: ";
        std::getline(std::cin, input);

        // Check jika input kosong
        if (input.empty()) {
            continue;
        }

        // Check jika user mau quit
        if (input == "quit") {
            std::cout << "[INFO] Exiting..." << std::endl;
            break;
        }

        // Tambahkan newline
        input += "\n";
        messageCount++;

        // Kirim datagram ke server (TIDAK perlu connect!)
        int bytesSent = sendto(
            clientSocket,
            input.c_str(),
            input.length(),
            0,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)
        );

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "[ERROR] sendto() failed!" << std::endl;
            std::cerr << "Pastikan server sudah running (udp_server.exe)" << std::endl;
            continue;
        }

        std::cout << "[SENT] Datagram #" << messageCount << " (" << bytesSent << " bytes)" << std::endl;

        // Terima response dari server
        memset(buffer, 0, BUFFER_SIZE);
        memset(&responseAddr, 0, sizeof(responseAddr));
        responseAddrLen = sizeof(responseAddr);

        int bytesReceived = recvfrom(
            clientSocket,
            buffer,
            BUFFER_SIZE - 1,
            0,
            (sockaddr*)&responseAddr,
            &responseAddrLen
        );

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Get server address dari response
            char serverIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(responseAddr.sin_addr), serverIP, INET_ADDRSTRLEN);
            int serverPort = ntohs(responseAddr.sin_port);

            std::cout << "[RECV] From " << serverIP << ":" << serverPort << std::endl;
            std::cout << "Server: " << buffer << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "[INFO] No data received" << std::endl;
        } else {
            std::cerr << "[ERROR] recvfrom() failed!" << std::endl;
        }
    }

    // 5. Cleanup
    std::cout << "\n[INFO] Closing socket..." << std::endl;
    closesocket(clientSocket);
    cleanupWinsock();

    std::cout << "[OK] Socket closed" << std::endl;
    std::cout << "\nTotal messages sent: " << messageCount << std::endl;
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}
