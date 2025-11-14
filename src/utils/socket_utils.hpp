/**
 * socket_utils.hpp - Common Socket Utility Functions
 *
 * File ini menyediakan helper functions yang sering digunakan dalam
 * socket programming untuk:
 * - Send/receive dengan error handling
 * - Buffer operations
 * - Address conversion
 * - Timeout operations
 *
 * Educational purposes - demonstrasi best practices
 */

#ifndef SOCKET_UTILS_HPP
#define SOCKET_UTILS_HPP

#include <string>
#include <cstring>

// Platform-specific includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>

    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

/**
 * Send all data (handle partial sends)
 *
 * send() mungkin tidak mengirim semua data sekaligus.
 * Function ini memastikan semua data terkirim.
 *
 * @param sock Socket descriptor
 * @param data Data to send
 * @param length Length of data
 * @return Number of bytes sent, or -1 on error
 */
inline int sendAll(SOCKET sock, const char* data, size_t length) {
    size_t totalSent = 0;

    while (totalSent < length) {
        int sent = send(sock, data + totalSent, length - totalSent, 0);

        if (sent == SOCKET_ERROR) {
            return -1;  // Error
        }

        totalSent += sent;
    }

    return totalSent;
}

/**
 * Send string dengan null terminator handling
 *
 * @param sock Socket descriptor
 * @param message String to send
 * @return true if success, false if error
 */
inline bool sendString(SOCKET sock, const std::string& message) {
    return sendAll(sock, message.c_str(), message.length()) >= 0;
}

/**
 * Receive data with timeout
 *
 * @param sock Socket descriptor
 * @param buffer Buffer to receive into
 * @param bufferSize Size of buffer
 * @param timeoutMs Timeout in milliseconds
 * @return Number of bytes received, 0 if timeout, -1 if error
 */
inline int receiveWithTimeout(SOCKET sock, char* buffer, size_t bufferSize, int timeoutMs) {
    fd_set readFds;
    FD_ZERO(&readFds);
    FD_SET(sock, &readFds);

    struct timeval timeout;
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;

    int result = select(sock + 1, &readFds, NULL, NULL, &timeout);

    if (result > 0 && FD_ISSET(sock, &readFds)) {
        return recv(sock, buffer, bufferSize, 0);
    } else if (result == 0) {
        return 0;  // Timeout
    } else {
        return -1;  // Error
    }
}

/**
 * Get peer address as string
 *
 * @param sock Socket descriptor
 * @return Address string "IP:PORT" or empty string on error
 */
inline std::string getPeerAddress(SOCKET sock) {
    sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(sock, (sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
        return "";
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);

    return std::string(ip) + ":" + std::to_string(port);
}

/**
 * Get local address as string
 *
 * @param sock Socket descriptor
 * @return Address string "IP:PORT" or empty string on error
 */
inline std::string getLocalAddress(SOCKET sock) {
    sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);

    if (getsockname(sock, (sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
        return "";
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);

    return std::string(ip) + ":" + std::to_string(port);
}

/**
 * Create TCP server socket (convenience function)
 *
 * @param port Port to bind to
 * @return Socket descriptor or INVALID_SOCKET on error
 */
inline SOCKET createTcpServerSocket(int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }

    // Set SO_REUSEADDR
    char opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return INVALID_SOCKET;
    }

    // Listen
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return INVALID_SOCKET;
    }

    return sock;
}

/**
 * Create UDP socket (convenience function)
 *
 * @param port Port to bind to (0 for auto-assign)
 * @return Socket descriptor or INVALID_SOCKET on error
 */
inline SOCKET createUdpSocket(int port = 0) {
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }

    if (port > 0) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return INVALID_SOCKET;
        }
    }

    return sock;
}

/**
 * Check if socket is still connected
 *
 * Uses select() dengan timeout 0 untuk check readability
 *
 * @param sock Socket descriptor
 * @return true if connected, false if disconnected
 */
inline bool isSocketConnected(SOCKET sock) {
    fd_set readFds;
    FD_ZERO(&readFds);
    FD_SET(sock, &readFds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int result = select(sock + 1, &readFds, NULL, NULL, &timeout);

    if (result > 0 && FD_ISSET(sock, &readFds)) {
        // Socket is readable - check if it's because of disconnect
        char buffer[1];
        int received = recv(sock, buffer, 1, MSG_PEEK);
        return received > 0;
    }

    return true;  // Assume connected if select returns 0 or error
}

/**
 * Example usage:
 *
 * // Send all data
 * std::string message = "Hello, World!";
 * if (sendString(socket, message)) {
 *     std::cout << "Message sent!" << std::endl;
 * }
 *
 * // Receive with timeout
 * char buffer[1024];
 * int received = receiveWithTimeout(socket, buffer, sizeof(buffer), 5000); // 5 second timeout
 * if (received > 0) {
 *     buffer[received] = '\0';
 *     std::cout << "Received: " << buffer << std::endl;
 * } else if (received == 0) {
 *     std::cout << "Timeout" << std::endl;
 * }
 *
 * // Get addresses
 * std::string peerAddr = getPeerAddress(socket);
 * std::cout << "Connected to: " << peerAddr << std::endl;
 *
 * // Create server easily
 * SOCKET server = createTcpServerSocket(8080);
 * if (server != INVALID_SOCKET) {
 *     std::cout << "Server listening on port 8080" << std::endl;
 * }
 */

#endif // SOCKET_UTILS_HPP
