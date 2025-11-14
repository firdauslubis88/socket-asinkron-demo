# CLAUDE.md - AI Assistant Guide for socket-asinkron-demo

## Project Overview

**Repository**: socket-asinkron-demo
**Purpose**: Demo program C++ untuk pembelajaran socket programming (TCP/UDP) dan proses asinkron
**Target Audience**: Mahasiswa Teknik Elektro Tahun 2, Fakultas Teknik Universitas Indonesia
**Duration**: 1 sesi perkuliahan (150 menit) - sifatnya pengenalan praktis tanpa latihan
**Last Updated**: 2025-11-14

### Konteks Proyek

Ini adalah program demo sederhana yang memberikan pengantar praktis tentang:
- Socket programming (TCP dan UDP) menggunakan C++
- Proses asinkron (non-blocking I/O)
- Format JSON untuk pertukaran data
- Praktik client-server communication

**Background Mahasiswa**:
- Telah mempelajari: C dasar (I/O, variabel, control flow, fungsi, array, pointer, file/JSON, linked list, searching, sorting)
- Telah mempelajari: OOP (abstraksi, enkapsulasi, pewarisan, polymorphism)
- **Belum** mempelajari: OSI layer secara mendalam
- Pendekatan: **Praktis** dan hands-on, bukan teoritis

---

## Repository Structure

```
socket-asinkron-demo/
├── src/
│   ├── tcp/                    # Implementasi TCP
│   │   ├── tcp_server.cpp      # Server TCP sederhana
│   │   └── tcp_client.cpp      # Client TCP sederhana
│   ├── udp/                    # Implementasi UDP
│   │   ├── udp_server.cpp      # Server UDP sederhana
│   │   └── udp_client.cpp      # Client UDP sederhana
│   ├── async/                  # Contoh proses asinkron
│   │   ├── async_tcp_server.cpp
│   │   └── async_tcp_client.cpp
│   ├── utils/                  # Utilitas umum
│   │   ├── json_helper.hpp     # Helper untuk JSON parsing
│   │   └── socket_utils.hpp    # Helper socket utilities
│   └── examples/               # Contoh lengkap kombinasi
│       └── chat_demo/          # Demo aplikasi chat sederhana
├── include/                    # Header files
├── lib/                        # Third-party libraries (nlohmann/json)
├── build/                      # Build output (ignored by git)
├── docs/                       # Dokumentasi tambahan
│   ├── penjelasan-tcp.md       # Penjelasan TCP untuk mahasiswa
│   ├── penjelasan-udp.md       # Penjelasan UDP untuk mahasiswa
│   └── penjelasan-async.md     # Penjelasan proses asinkron
├── Makefile                    # Build configuration
├── CMakeLists.txt             # CMake configuration (optional)
├── README.md                   # Dokumentasi utama
├── .gitignore                 # Git ignore rules
└── CLAUDE.md                  # File ini
```

---

## Technology Stack

### Core Technologies
- **Language**: C++11 atau lebih baru (untuk fitur modern seperti `std::thread`, `std::async`)
- **Socket API**: **Winsock2** (Windows) - Primary target platform
- **JSON Library**: [nlohmann/json](https://github.com/nlohmann/json) - single header, mudah digunakan
- **Build System**: MinGW/MSVC untuk Windows, atau CMake (cross-platform)
- **Compiler**: MinGW g++ atau MSVC (Visual Studio)

### Key Libraries

#### Windows (Winsock2)
```cpp
// Windows-specific headers
#include <winsock2.h>      // Winsock2 API
#include <ws2tcpip.h>      // Additional TCP/IP functions
#pragma comment(lib, "ws2_32.lib")  // Link dengan Winsock library

// Standard C++ libraries
#include <thread>          // Multi-threading
#include <future>          // std::async, std::future
#include <chrono>          // Time utilities
#include <iostream>        // I/O
#include <string>          // String handling
#include <vector>          // Dynamic arrays

// Third-party
#include "json.hpp"        // nlohmann/json
```

#### Linux/Unix (untuk referensi)
```cpp
// POSIX headers (jika develop di Linux)
#include <sys/socket.h>    // Socket programming
#include <netinet/in.h>    // Internet address family
#include <arpa/inet.h>     // inet_ntoa, inet_addr
#include <unistd.h>        // close()
#include <fcntl.h>         // fcntl() untuk non-blocking
```

### Platform Compatibility Layer

Untuk memudahkan cross-platform development:

```cpp
// socket_compat.hpp - Compatibility layer
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    typedef int socklen_t;
    #define CLOSE_SOCKET closesocket
    #define GET_ERROR WSAGetLastError()
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET close
    #define GET_ERROR errno
#endif
```

---

## Development Workflow

### Git Branching Strategy

1. **Main Branch**: `main` - kode stabil dan siap presentasi
2. **Development Branch**: `develop` - integrasi fitur baru
3. **Feature Branches**: `feature/nama-fitur` - pengembangan fitur spesifik
4. **Claude Branches**: `claude/claude-md-*` - AI-assisted development

### Commit Conventions

Gunakan format conventional commits:
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**:
- `feat`: Fitur baru (contoh: feat(tcp): add basic TCP server)
- `fix`: Bug fix
- `docs`: Perubahan dokumentasi
- `refactor`: Refactoring kode
- `chore`: Maintenance tasks
- `example`: Menambah contoh baru

**Contoh**:
```
feat(tcp): implement basic TCP echo server

Add simple TCP server yang menerima koneksi client dan
mengirim kembali pesan yang diterima (echo).

feat(async): add non-blocking socket example

Implement contoh socket non-blocking menggunakan fcntl()
untuk demonstrasi proses asinkron.
```

---

## Code Conventions for C++

### 1. Naming Conventions

```cpp
// Classes: PascalCase
class TcpServer { };
class JsonHelper { };

// Functions: snake_case
void send_message(int socket_fd, const std::string& msg);
int create_socket(int port);

// Variables: snake_case
int server_socket;
std::string client_address;
const int MAX_BUFFER_SIZE = 1024;

// Constants: UPPER_SNAKE_CASE
const int DEFAULT_PORT = 8080;
const int MAX_CONNECTIONS = 10;
```

### 2. Code Style

```cpp
// Gunakan const reference untuk parameter yang tidak diubah
void process_data(const std::string& data);

// RAII untuk resource management
class SocketWrapper {
    int fd_;
public:
    SocketWrapper(int fd) : fd_(fd) {}
    ~SocketWrapper() { if (fd_ >= 0) close(fd_); }
    // ... copy/move semantics
};

// Prefer early return
int send_data(int socket, const char* data, size_t len) {
    if (socket < 0) return -1;
    if (data == nullptr) return -1;
    if (len == 0) return 0;

    return send(socket, data, len, 0);
}

// Clear error handling
int result = socket(AF_INET, SOCK_STREAM, 0);
if (result < 0) {
    std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    return -1;
}
```

### 3. Comments dan Documentation

```cpp
/**
 * Membuat TCP server socket dan bind ke port yang ditentukan.
 *
 * @param port Port number untuk listening (1024-65535)
 * @return Socket file descriptor jika sukses, -1 jika gagal
 *
 * Contoh penggunaan:
 *   int server_fd = create_tcp_server(8080);
 *   if (server_fd < 0) {
 *       // handle error
 *   }
 */
int create_tcp_server(int port);

// Inline comments untuk logika kompleks
// Set socket ke non-blocking mode menggunakan fcntl
int flags = fcntl(socket_fd, F_GETFL, 0);
fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
```

---

## Key Programming Patterns

### 1. Basic TCP Server Pattern (Windows)

```cpp
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

SOCKET create_tcp_server(int port) {
    // 0. Initialize Winsock (PENTING! Wajib di Windows)
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    // 1. Buat socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 2. Set socket options (optional, untuk reuse address)
    char opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Bind ke address dan port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 4. Listen untuk koneksi
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return INVALID_SOCKET;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return server_fd;
}

// Jangan lupa cleanup!
void cleanup_server(SOCKET server_fd) {
    closesocket(server_fd);
    WSACleanup();
}
```

### 2. Basic TCP Client Pattern (Windows)

```cpp
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

SOCKET connect_to_server(const char* ip, int port) {
    // 0. Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    // 1. Buat socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 2. Set server address
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IP address from string to binary
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << ip << std::endl;
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 3. Connect ke server
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    std::cout << "Connected to server " << ip << ":" << port << std::endl;
    return sock;
}

// Cleanup
void cleanup_client(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}
```

### 3. UDP Socket Pattern

```cpp
// UDP Server
int create_udp_server(int port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(sock);
        return -1;
    }

    return sock;
}

// UDP receive
ssize_t receive_udp(int sock, char* buffer, size_t len, struct sockaddr_in* client_addr) {
    socklen_t addr_len = sizeof(*client_addr);
    return recvfrom(sock, buffer, len, 0, (struct sockaddr*)client_addr, &addr_len);
}
```

### 4. Async/Non-Blocking Pattern

```cpp
// Set socket ke non-blocking
void set_nonblocking(int socket_fd) {
    int flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
}

// Async receive dengan timeout
bool receive_with_timeout(int sock, char* buffer, size_t len, int timeout_ms) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int result = select(sock + 1, &read_fds, nullptr, nullptr, &timeout);

    if (result > 0 && FD_ISSET(sock, &read_fds)) {
        recv(sock, buffer, len, 0);
        return true;
    }
    return false;  // Timeout atau error
}
```

### 5. JSON Message Pattern

```cpp
#include "json.hpp"
using json = nlohmann::json;

// Membuat JSON message
std::string create_json_message(const std::string& type, const std::string& content) {
    json msg;
    msg["type"] = type;
    msg["content"] = content;
    msg["timestamp"] = std::time(nullptr);
    return msg.dump();
}

// Parse JSON message
void parse_json_message(const std::string& json_str) {
    try {
        json msg = json::parse(json_str);
        std::string type = msg["type"];
        std::string content = msg["content"];

        std::cout << "Type: " << type << ", Content: " << content << std::endl;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
}
```

---

## AI Assistant Guidelines

### When Creating Examples

1. **Keep It Simple**
   - Mahasiswa baru belajar socket programming
   - Fokus pada konsep dasar, bukan fitur advanced
   - Setiap contoh harus bisa dijelaskan dalam 10-15 menit

2. **Make It Practical**
   - Setiap contoh harus bisa di-compile dan dijalankan
   - Include output yang jelas untuk debugging
   - Berikan contoh use case yang relatable (chat, echo server, dll)

3. **Build Incrementally**
   - Mulai dari TCP sederhana → UDP → Async
   - Setiap level menambah kompleksitas sedikit
   - Reuse code patterns yang sudah dijelaskan sebelumnya

4. **Focus on Learning**
   - Tambahkan comments yang menjelaskan "mengapa", bukan hanya "apa"
   - Highlight perbedaan antara TCP vs UDP
   - Explain trade-offs dari setiap pendekatan

### Code Simplicity Principles

```cpp
// ✅ GOOD - Simple, clear, educational
void handle_client(int client_sock) {
    char buffer[1024];
    int bytes_read = recv(client_sock, buffer, sizeof(buffer), 0);

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // Echo back ke client
        send(client_sock, buffer, bytes_read, 0);
    }
}

// ❌ AVOID - Too complex untuk demo
template<typename MessageHandler>
class AsyncSocketManager {
    std::unordered_map<int, std::shared_ptr<Connection>> connections_;
    std::thread_pool pool_;
    // ... terlalu banyak abstraction untuk mahasiswa tahun 2
};
```

### Documentation Style for Students

```cpp
/**
 * TCP SERVER - Contoh sederhana
 *
 * Program ini membuat server TCP yang:
 * 1. Listen pada port 8080
 * 2. Menerima koneksi dari client
 * 3. Menerima pesan dari client
 * 4. Mengirim kembali pesan yang sama (echo)
 *
 * Cara menjalankan:
 *   1. Compile: g++ tcp_server.cpp -o server
 *   2. Run: ./server
 *   3. Di terminal lain, jalankan client atau gunakan telnet:
 *      telnet localhost 8080
 *
 * Konsep yang didemonstrasikan:
 * - Socket creation (socket())
 * - Binding ke port (bind())
 * - Listening untuk koneksi (listen())
 * - Accepting client connection (accept())
 * - Receiving data (recv())
 * - Sending data (send())
 */
```

---

## Build Instructions

### Using Makefile

```makefile
# Makefile example
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
LDFLAGS = -pthread

# TCP examples
tcp_server: src/tcp/tcp_server.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

tcp_client: src/tcp/tcp_client.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

# UDP examples
udp_server: src/udp/udp_server.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

udp_client: src/udp/udp_client.cpp
	$(CXX) $(CXXFLAGS) $^ -o bin/$@ $(LDFLAGS)

all: tcp_server tcp_client udp_server udp_client

clean:
	rm -f bin/*
```

### Using CMake (Optional)

```cmake
cmake_minimum_required(VERSION 3.10)
project(SocketAsinkronDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# TCP examples
add_executable(tcp_server src/tcp/tcp_server.cpp)
add_executable(tcp_client src/tcp/tcp_client.cpp)

# UDP examples
add_executable(udp_server src/udp/udp_server.cpp)
add_executable(udp_client src/udp/udp_client.cpp)
```

---

## Testing Strategy

### Manual Testing

Karena ini demo educational, fokus pada manual testing yang bisa dilakukan live di kelas:

1. **TCP Echo Test**
   ```bash
   # Terminal 1
   ./tcp_server

   # Terminal 2
   ./tcp_client
   # Atau gunakan telnet
   telnet localhost 8080
   ```

2. **UDP Test**
   ```bash
   # Terminal 1
   ./udp_server

   # Terminal 2
   ./udp_client
   ```

3. **JSON Message Test**
   ```bash
   # Send JSON message dan lihat parsing di server
   echo '{"type":"message","content":"hello"}' | nc localhost 8080
   ```

### Debugging Tips

```cpp
// Helper macro untuk debugging
#define DEBUG_PRINT(x) std::cout << "[DEBUG] " << __FUNCTION__ << ": " << x << std::endl

// Contoh penggunaan
DEBUG_PRINT("Socket created with fd: " << socket_fd);
DEBUG_PRINT("Received " << bytes_read << " bytes");
```

---

## Common Issues and Solutions

### 1. "Address already in use"
```cpp
// Solution: Set SO_REUSEADDR before bind
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

### 2. "Connection refused"
```cpp
// Check:
// 1. Apakah server sudah running?
// 2. Apakah port number benar?
// 3. Apakah firewall memblokir port?
```

### 3. Buffer handling
```cpp
// Selalu null-terminate string dari recv()
char buffer[1024];
int bytes_read = recv(sock, buffer, sizeof(buffer) - 1, 0);
if (bytes_read > 0) {
    buffer[bytes_read] = '\0';  // PENTING!
    std::cout << buffer << std::endl;
}
```

---

## Demo Sequence (150 menit)

### Segment 1: Pengantar (15 menit)
- Apa itu socket?
- Client-Server architecture
- TCP vs UDP overview

### Segment 2: TCP Demo (45 menit)
- Basic TCP server
- Basic TCP client
- Echo server demo
- Live debugging

### Segment 3: UDP Demo (30 menit)
- Basic UDP server
- Basic UDP client
- Perbedaan dengan TCP
- Use cases

### Segment 4: Async + JSON (45 menit)
- Non-blocking sockets
- Select/poll untuk multiple clients
- JSON message format
- Simple chat demo

### Segment 5: Q&A (15 menit)
- Tanya jawab
- Tips untuk belajar lebih lanjut

---

## Learning Resources for Students

### Recommended Reading
1. Beej's Guide to Network Programming (free online)
2. "UNIX Network Programming" by W. Richard Stevens (advanced)
3. C++ Socket Programming tutorials (cppreference.com)

### Online Tools
- `netcat` (nc) - untuk testing socket
- `telnet` - untuk testing TCP
- Wireshark - untuk melihat network packets
- `netstat` / `ss` - untuk melihat active connections

---

## Security Notes

Untuk demo ini, security tidak menjadi fokus utama, tapi perhatikan:

```cpp
// ✅ Good practice bahkan untuk demo
// 1. Validate buffer sizes
if (bytes_read >= sizeof(buffer)) {
    // Handle error
}

// 2. Check return values
if (send(sock, data, len, 0) < 0) {
    perror("send failed");
}

// 3. Close sockets properly
close(socket_fd);
```

**Catatan**: Di production code, perlu tambahan: input validation, authentication, encryption (SSL/TLS), dll.

---

## Changelog

### 2025-11-14 - Initial Setup
- Created CLAUDE.md dengan konteks C++ educational demo
- Defined struktur proyek untuk socket programming demo
- Established code conventions untuk C++
- Added patterns dan examples untuk TCP/UDP/Async
- Documented demo sequence untuk 150 menit lecture

---

**End of Document**

*Dokumen ini adalah panduan untuk AI assistant dalam mengembangkan kode demo. Update ketika ada perubahan signifikan pada struktur atau requirements.*
