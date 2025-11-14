# Penjelasan Proses Asinkron (Asynchronous Programming)

## Apa itu Proses Asinkron?

Proses asinkron adalah cara menjalankan operasi **tanpa blocking** (menunggu). Program bisa melakukan hal lain sambil menunggu operasi selesai.

## Analogi Sederhana

### Synchronous (Blocking)
Seperti **antre di kasir**:
1. Kasir 1 layani customer A
2. Customer B, C, D harus **tunggu** sampai A selesai
3. Satu per satu, berurutan

```
Kasir: [A]----[B]----[C]----[D]----
       ↑ tunggu!
```

### Asynchronous (Non-blocking)
Seperti **restoran dengan buzzer**:
1. Customer A pesan, dapat buzzer, boleh duduk dulu
2. Customer B pesan, dapat buzzer, boleh duduk dulu
3. Koki masak semua pesanan **bersamaan**
4. Buzzer bunyi kalau pesanan siap

```
Koki:  [A][B][C][D]  (semua diproses bersamaan)
       ↑ tidak perlu tunggu!
```

## Mengapa Perlu Asinkron di Socket?

### Problem: Blocking I/O

```cpp
// Server sederhana dengan blocking
while (true) {
    int client = accept(server_sock, ...);  // BLOCK disini
    char buffer[1024];
    recv(client, buffer, sizeof(buffer), 0);  // BLOCK lagi disini
    // Selama recv() blocking, tidak bisa terima client lain!
}
```

**Masalah**:
- Server hanya bisa handle 1 client pada satu waktu
- Client lain harus tunggu
- Tidak efisien!

### Solution: Non-blocking I/O

```cpp
// Set socket ke non-blocking
set_nonblocking(server_sock);

while (true) {
    int client = accept(server_sock, ...);
    if (client < 0) {
        if (errno == EAGAIN) {
            // Tidak ada client baru, lanjut
            continue;
        }
    } else {
        // Ada client baru, proses
        handle_new_client(client);
    }

    // Cek semua client yang sudah connect
    check_existing_clients();
}
```

## Teknik Asinkron di C++

### 1. Non-blocking Sockets

```cpp
#include <fcntl.h>

void set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}
```

**Efek**: Operasi socket return immediately, tidak tunggu.

### 2. Select/Poll/Epoll

Monitor multiple sockets sekaligus.

#### select() - Classic approach

```cpp
fd_set read_fds;
FD_ZERO(&read_fds);
FD_SET(sock1, &read_fds);
FD_SET(sock2, &read_fds);
FD_SET(sock3, &read_fds);

struct timeval timeout;
timeout.tv_sec = 1;
timeout.tv_usec = 0;

// Wait sampai ada socket yang ready untuk read
int ready = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

if (ready > 0) {
    if (FD_ISSET(sock1, &read_fds)) {
        // sock1 ready untuk read
        recv(sock1, ...);
    }
    if (FD_ISSET(sock2, &read_fds)) {
        // sock2 ready untuk read
        recv(sock2, ...);
    }
    // dst...
}
```

**Keuntungan**: Bisa monitor banyak sockets dengan 1 thread!

#### poll() - Modern alternative

```cpp
struct pollfd fds[MAX_CLIENTS];
fds[0].fd = sock1;
fds[0].events = POLLIN;  // Interested in reading
fds[1].fd = sock2;
fds[1].events = POLLIN;

int ready = poll(fds, num_fds, timeout_ms);

for (int i = 0; i < num_fds; i++) {
    if (fds[i].revents & POLLIN) {
        // fds[i].fd ready untuk read
        recv(fds[i].fd, ...);
    }
}
```

#### epoll() - Linux specific, most efficient

```cpp
int epoll_fd = epoll_create1(0);

struct epoll_event event;
event.events = EPOLLIN;
event.data.fd = sock;

epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event);

struct epoll_event events[MAX_EVENTS];
int num_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout_ms);

for (int i = 0; i < num_ready; i++) {
    // events[i].data.fd ready
}
```

### 3. Multi-threading

```cpp
#include <thread>

void handle_client(int client_sock) {
    // Handle client di thread terpisah
    char buffer[1024];
    while (true) {
        int bytes = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        send(client_sock, buffer, bytes, 0);
    }
    close(client_sock);
}

// Main server loop
while (true) {
    int client = accept(server_sock, ...);
    if (client >= 0) {
        // Spawn thread baru untuk setiap client
        std::thread t(handle_client, client);
        t.detach();  // Let it run independently
    }
}
```

**Catatan**: Thread bisa expensive jika banyak clients (lihat thread pools).

### 4. std::async (C++11)

```cpp
#include <future>

void handle_client(int client_sock) {
    // ...
}

// Launch asynchronously
std::future<void> result = std::async(std::launch::async, handle_client, client_sock);
```

## Comparison Table

| Teknik | Complexity | Scalability | Platform |
|--------|-----------|-------------|----------|
| Blocking | ⭐ Simple | ❌ Poor | All |
| Non-blocking | ⭐⭐ Medium | ⭐⭐ Good | All |
| select() | ⭐⭐ Medium | ⭐⭐ Good (limit 1024) | All |
| poll() | ⭐⭐ Medium | ⭐⭐⭐ Better | Unix |
| epoll() | ⭐⭐⭐ Complex | ⭐⭐⭐⭐ Best | Linux |
| Threads | ⭐⭐ Medium | ⭐⭐ OK (overhead) | All |

## Praktik Terbaik

### 1. Handle EAGAIN/EWOULDBLOCK

```cpp
int bytes = recv(sock, buffer, sizeof(buffer), 0);
if (bytes < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // Not an error, just no data available yet
        // Try again later
        return;
    } else {
        // Real error
        perror("recv failed");
    }
}
```

### 2. Timeout untuk Non-blocking Operations

```cpp
bool recv_with_timeout(int sock, char* buffer, size_t len, int timeout_ms) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(sock + 1, &read_fds, NULL, NULL, &tv);

    if (ret > 0 && FD_ISSET(sock, &read_fds)) {
        recv(sock, buffer, len, 0);
        return true;
    }

    return false;  // Timeout
}
```

### 3. Connection Management

```cpp
struct ClientConnection {
    int socket_fd;
    time_t last_activity;
    std::string buffer;  // Partial data
};

std::vector<ClientConnection> clients;

// Cleanup idle connections
void cleanup_idle_connections(int timeout_seconds) {
    time_t now = time(nullptr);
    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
            [now, timeout_seconds](const ClientConnection& c) {
                if (now - c.last_activity > timeout_seconds) {
                    close(c.socket_fd);
                    return true;
                }
                return false;
            }),
        clients.end()
    );
}
```

## Event-Driven Architecture

```
+------------------+
|   Event Loop     |
+------------------+
        |
        v
+------------------+
| Wait for events  | <--- select()/poll()/epoll()
+------------------+
        |
        v
+------------------+
| Process events   |
+------------------+
   |    |    |
   v    v    v
 New  Read  Write
Client Data  Data
```

### Pseudo-code

```cpp
while (server_running) {
    // 1. Wait untuk events
    events = wait_for_events(timeout);

    // 2. Process setiap event
    for (event in events) {
        if (event.type == NEW_CONNECTION) {
            handle_new_connection();
        } else if (event.type == DATA_AVAILABLE) {
            handle_data_available(event.socket);
        } else if (event.type == CAN_WRITE) {
            handle_can_write(event.socket);
        } else if (event.type == DISCONNECT) {
            handle_disconnect(event.socket);
        }
    }

    // 3. Cleanup
    cleanup_closed_connections();
}
```

## Real-world Example: Chat Server

```cpp
// Manage banyak clients secara asinkron
class AsyncChatServer {
    int server_sock;
    std::map<int, std::string> clients;  // fd -> username

public:
    void run() {
        set_nonblocking(server_sock);

        while (true) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(server_sock, &read_fds);

            int max_fd = server_sock;

            // Add all client sockets
            for (auto& [fd, name] : clients) {
                FD_SET(fd, &read_fds);
                max_fd = std::max(max_fd, fd);
            }

            // Wait untuk activity
            struct timeval tv = {1, 0};
            int activity = select(max_fd + 1, &read_fds, NULL, NULL, &tv);

            if (activity < 0) continue;

            // Check new connection
            if (FD_ISSET(server_sock, &read_fds)) {
                int new_client = accept(server_sock, NULL, NULL);
                if (new_client >= 0) {
                    set_nonblocking(new_client);
                    clients[new_client] = "User" + std::to_string(new_client);
                    broadcast(clients[new_client] + " joined!\n");
                }
            }

            // Check existing clients
            for (auto it = clients.begin(); it != clients.end(); ) {
                int fd = it->first;

                if (FD_ISSET(fd, &read_fds)) {
                    char buffer[1024];
                    int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

                    if (bytes <= 0) {
                        // Client disconnected
                        broadcast(it->second + " left!\n");
                        close(fd);
                        it = clients.erase(it);
                        continue;
                    }

                    buffer[bytes] = '\0';
                    broadcast(it->second + ": " + buffer);
                }

                ++it;
            }
        }
    }

    void broadcast(const std::string& msg) {
        for (auto& [fd, name] : clients) {
            send(fd, msg.c_str(), msg.length(), 0);
        }
    }
};
```

## Performance Tips

1. **Use epoll() on Linux** untuk best performance dengan banyak connections
2. **Connection pooling** untuk reuse connections
3. **Buffer management** gunakan buffer pool untuk avoid allocation overhead
4. **Thread pools** daripada spawn thread baru setiap request
5. **Monitoring** track metrics (connections, throughput, latency)

## Common Pitfalls

❌ **Lupa handle EAGAIN**
```cpp
// BAD
recv(sock, buffer, size, 0);  // Treat all errors as fatal

// GOOD
if (recv(sock, buffer, size, 0) < 0 && errno != EAGAIN) {
    // Handle real error
}
```

❌ **Tidak cleanup connections**
```cpp
// Resource leak jika tidak close sockets yang disconnect
```

❌ **Busy waiting**
```cpp
// BAD - waste CPU
while (true) {
    if (data_available(sock)) {
        recv(sock, ...);
    }
}

// GOOD - wait efficiently
select(sock, timeout);
```

## Resources

- Book: "UNIX Network Programming" - Stevens
- Linux man pages: `man 2 select`, `man 2 poll`, `man 7 epoll`
- libuv - Cross-platform async I/O library
- Boost.Asio - C++ async networking

---

**Key Takeaway**: Asinkron programming memungkinkan server handle banyak clients efficiently tanpa perlu banyak threads atau blocking!
