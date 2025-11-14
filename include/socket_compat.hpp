/**
 * socket_compat.hpp - Cross-platform Socket Compatibility Layer
 *
 * File ini menyediakan abstraction layer untuk socket programming
 * yang bekerja di Windows (Winsock) dan Linux/Unix (POSIX sockets)
 *
 * Author: Demo untuk Mahasiswa Teknik Elektro UI
 * Platform: Windows (primary), Linux (secondary)
 */

#ifndef SOCKET_COMPAT_HPP
#define SOCKET_COMPAT_HPP

#include <iostream>
#include <string>
#include <cstring>

// Platform detection dan includes
#ifdef _WIN32
    // Windows - menggunakan Winsock2
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    // Type aliases untuk compatibility
    typedef int socklen_t;

    // Function aliases
    #define CLOSE_SOCKET closesocket
    #define GET_LAST_ERROR() WSAGetLastError()

#else
    // Linux/Unix - menggunakan POSIX sockets
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>

    // Type aliases untuk compatibility
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1

    // Function aliases
    #define CLOSE_SOCKET close
    #define GET_LAST_ERROR() errno

#endif

/**
 * Initialize socket library
 * Di Windows: Initialize Winsock
 * Di Linux: No-op (tidak perlu initialization)
 *
 * @return true jika sukses, false jika gagal
 */
inline bool socket_init() {
#ifdef _WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }
    return true;
#else
    return true;  // No initialization needed on Unix
#endif
}

/**
 * Cleanup socket library
 * Di Windows: Cleanup Winsock
 * Di Linux: No-op
 */
inline void socket_cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

/**
 * Get last socket error as string
 *
 * @return Error message string
 */
inline std::string socket_error_string() {
#ifdef _WIN32
    int error = WSAGetLastError();
    char* msg = nullptr;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msg,
        0,
        NULL
    );

    std::string result = msg ? msg : "Unknown error";
    if (msg) LocalFree(msg);

    return result + " (Error code: " + std::to_string(error) + ")";
#else
    return std::string(strerror(errno)) + " (Error code: " + std::to_string(errno) + ")";
#endif
}

/**
 * Set socket to non-blocking mode
 *
 * @param sock Socket file descriptor
 * @return true jika sukses, false jika gagal
 */
inline bool set_nonblocking(SOCKET sock) {
#ifdef _WIN32
    u_long mode = 1;  // 1 = non-blocking, 0 = blocking
    if (ioctlsocket(sock, FIONBIO, &mode) != 0) {
        std::cerr << "ioctlsocket failed: " << socket_error_string() << std::endl;
        return false;
    }
    return true;
#else
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return false;

    flags |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags) == -1) {
        std::cerr << "fcntl failed: " << socket_error_string() << std::endl;
        return false;
    }
    return true;
#endif
}

/**
 * Set socket to blocking mode
 *
 * @param sock Socket file descriptor
 * @return true jika sukses, false jika gagal
 */
inline bool set_blocking(SOCKET sock) {
#ifdef _WIN32
    u_long mode = 0;  // 0 = blocking, 1 = non-blocking
    if (ioctlsocket(sock, FIONBIO, &mode) != 0) {
        std::cerr << "ioctlsocket failed: " << socket_error_string() << std::endl;
        return false;
    }
    return true;
#else
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return false;

    flags &= ~O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags) == -1) {
        std::cerr << "fcntl failed: " << socket_error_string() << std::endl;
        return false;
    }
    return true;
#endif
}

/**
 * Check if error is "would block" error
 * Berguna untuk non-blocking sockets
 *
 * @return true jika error adalah EWOULDBLOCK/EAGAIN
 */
inline bool is_would_block_error() {
#ifdef _WIN32
    return (WSAGetLastError() == WSAEWOULDBLOCK);
#else
    return (errno == EWOULDBLOCK || errno == EAGAIN);
#endif
}

/**
 * RAII wrapper untuk socket initialization
 * Otomatis initialize di constructor dan cleanup di destructor
 *
 * Contoh penggunaan:
 *   SocketInitializer init;
 *   // ... gunakan sockets ...
 *   // Otomatis cleanup saat keluar dari scope
 */
class SocketInitializer {
public:
    SocketInitializer() {
        if (!socket_init()) {
            throw std::runtime_error("Failed to initialize socket library");
        }
    }

    ~SocketInitializer() {
        socket_cleanup();
    }

    // Prevent copying
    SocketInitializer(const SocketInitializer&) = delete;
    SocketInitializer& operator=(const SocketInitializer&) = delete;
};

/**
 * RAII wrapper untuk socket file descriptor
 * Otomatis close socket di destructor
 *
 * Contoh penggunaan:
 *   SocketWrapper sock(socket(AF_INET, SOCK_STREAM, 0));
 *   // ... gunakan sock.get() ...
 *   // Otomatis close saat keluar dari scope
 */
class SocketWrapper {
    SOCKET sock_;

public:
    explicit SocketWrapper(SOCKET sock = INVALID_SOCKET) : sock_(sock) {}

    ~SocketWrapper() {
        close();
    }

    // Get socket descriptor
    SOCKET get() const { return sock_; }

    // Check if socket is valid
    bool is_valid() const { return sock_ != INVALID_SOCKET; }

    // Close socket manually
    void close() {
        if (is_valid()) {
            CLOSE_SOCKET(sock_);
            sock_ = INVALID_SOCKET;
        }
    }

    // Transfer ownership (for returning from functions)
    SOCKET release() {
        SOCKET temp = sock_;
        sock_ = INVALID_SOCKET;
        return temp;
    }

    // Prevent copying
    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;

    // Allow moving
    SocketWrapper(SocketWrapper&& other) noexcept : sock_(other.sock_) {
        other.sock_ = INVALID_SOCKET;
    }

    SocketWrapper& operator=(SocketWrapper&& other) noexcept {
        if (this != &other) {
            close();
            sock_ = other.sock_;
            other.sock_ = INVALID_SOCKET;
        }
        return *this;
    }
};

#endif // SOCKET_COMPAT_HPP
