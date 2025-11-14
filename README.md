# Socket Asinkron Demo - C++ Socket Programming

Demo program C++ untuk pembelajaran socket programming (TCP/UDP) dan proses asinkron.

## 📚 Tentang Project

Project ini adalah materi pembelajaran untuk mahasiswa Teknik Elektro tahun kedua Fakultas Teknik Universitas Indonesia. Tujuannya adalah memberikan pengantar praktis tentang:

- **Socket Programming**: Komunikasi jaringan menggunakan TCP dan UDP
- **Proses Asinkron**: Non-blocking I/O dan concurrent connections
- **Format JSON**: Pertukaran data terstruktur antar client-server
- **Client-Server Architecture**: Pola komunikasi client-server

## 🎯 Target Audience

Mahasiswa yang telah mempelajari:
- ✅ Dasar C (I/O, variabel, control flow, fungsi, array, pointer)
- ✅ File handling dan JSON
- ✅ Data structures (linked list, searching, sorting)
- ✅ OOP (abstraksi, enkapsulasi, pewarisan, polymorphism)

## 📁 Struktur Project

```
socket-asinkron-demo/
├── src/
│   ├── tcp/              # Implementasi TCP (server & client)
│   ├── udp/              # Implementasi UDP (server & client)
│   ├── async/            # Contoh proses asinkron
│   ├── utils/            # Helper utilities
│   └── examples/         # Contoh aplikasi lengkap
├── include/              # Header files
├── docs/                 # Dokumentasi tambahan
├── Makefile              # Build configuration
└── README.md             # File ini
```

## 🚀 Quick Start

### Prerequisites

#### Windows (Platform Utama)
- **Compiler**: MinGW g++ atau MSVC (Visual Studio)
- **OS**: Windows 10/11
- **Tools**: git (optional)
- **Winsock**: Sudah include di Windows

Lihat [Windows Setup Guide](docs/windows-setup.md) untuk detailed installation instructions.

#### Linux/Unix (Optional)
- **Compiler**: g++ atau clang++ dengan support C++11
- **OS**: Linux/Unix atau WSL
- **Tools**: make, git

### Instalasi

#### Windows

```cmd
# Clone repository
git clone <repository-url>
cd socket-asinkron-demo

# Build semua contoh dengan batch script
build.bat

# Atau build individual
g++ -std=c++11 -Iinclude src/tcp/tcp_server.cpp -o bin/tcp_server.exe -lws2_32
g++ -std=c++11 -Iinclude src/tcp/tcp_client.cpp -o bin/tcp_client.exe -lws2_32
```

#### Linux (jika develop di Linux)

```bash
# Clone repository
git clone <repository-url>
cd socket-asinkron-demo

# Build semua contoh
make all

# Atau build individual
make tcp_server
make tcp_client
```

### Menjalankan Demo

#### Windows

**TCP Echo Server**
```cmd
REM Terminal 1 (Command Prompt): Jalankan server
cd bin
tcp_server.exe

REM Terminal 2 (Command Prompt baru): Jalankan client
cd bin
tcp_client.exe

REM Atau gunakan telnet (install dulu jika belum ada)
telnet localhost 8080
```

**UDP Server**
```cmd
REM Terminal 1: Jalankan server
cd bin
udp_server.exe

REM Terminal 2: Jalankan client
cd bin
udp_client.exe
```

**Async Server**
```cmd
REM Terminal 1: Jalankan server (handle multiple clients)
cd bin
async_server.exe

REM Terminal 2 & 3: Jalankan multiple clients
cd bin
async_client.exe
```

#### Linux/Unix

**TCP Echo Server**
```bash
# Terminal 1: Jalankan server
./bin/tcp_server

# Terminal 2: Jalankan client
./bin/tcp_client
# Atau gunakan telnet
telnet localhost 8080
```

**UDP Server**
```bash
# Terminal 1: Jalankan server
./bin/udp_server

# Terminal 2: Jalankan client
./bin/udp_client
```

## 📖 Materi Pembelajaran

### 1. TCP Socket Programming
- Membuat TCP server dan client
- Connection-oriented communication
- Reliable data transfer
- Echo server implementation

### 2. UDP Socket Programming
- Membuat UDP server dan client
- Connectionless communication
- Datagram-based transfer
- Perbandingan dengan TCP

### 3. Asynchronous Processing
- Non-blocking sockets
- Select/poll untuk multiple clients
- Concurrent connections handling
- Event-driven programming

### 4. JSON Message Format
- Parsing dan generating JSON
- Structured data exchange
- Protocol design
- Error handling

## 🛠️ Build Commands

### Windows

```cmd
REM Build semua dengan batch script
build.bat

REM Build individual dengan g++ (MinGW)
g++ -std=c++11 -Iinclude src\tcp\tcp_server.cpp -o bin\tcp_server.exe -lws2_32
g++ -std=c++11 -Iinclude src\tcp\tcp_client.cpp -o bin\tcp_client.exe -lws2_32
g++ -std=c++11 -Iinclude src\udp\udp_server.cpp -o bin\udp_server.exe -lws2_32
g++ -std=c++11 -Iinclude src\udp\udp_client.cpp -o bin\udp_client.exe -lws2_32

REM Clean (hapus semua executables)
del /Q bin\*.exe
```

**Note**: Flag `-lws2_32` penting untuk link dengan Winsock library!

### Linux

```bash
# Build semua
make all

# Build individual components
make tcp_server      # TCP server
make tcp_client      # TCP client
make udp_server      # UDP server
make udp_client      # UDP client
make async_server    # Async server
make chat_demo       # Chat application demo

# Clean build files
make clean
```

## 📚 Dokumentasi

Dokumentasi lengkap tersedia di folder `docs/`:
- [Penjelasan TCP](docs/penjelasan-tcp.md)
- [Penjelasan UDP](docs/penjelasan-udp.md)
- [Penjelasan Async](docs/penjelasan-async.md)

## 🔍 Troubleshooting

### Windows

#### Error: "g++ is not recognized"
MinGW belum terinstall atau belum di PATH.

**Solusi**: Lihat [Windows Setup Guide](docs/windows-setup.md)

#### Error: "undefined reference to WSAStartup"
Lupa link dengan Winsock library.

**Solusi**: Tambahkan flag `-lws2_32`:
```cmd
g++ program.cpp -o program.exe -lws2_32
```

#### Error: "Address already in use"
Port masih digunakan program lain.

**Solusi**:
```cmd
REM Check port usage
netstat -ano | findstr :8080

REM Kill process (ganti PID dengan angka dari netstat)
taskkill /F /PID <process_id>
```

#### Firewall Blocking Port
Windows Firewall memblokir connection.

**Solusi**:
- Windows Security → Firewall → Advanced settings
- Inbound Rules → New Rule → Port (8080) → Allow

### Linux

#### Error: "Address already in use"
```bash
# Tunggu beberapa detik atau gunakan port berbeda
# Atau kill process yang menggunakan port
lsof -ti:8080 | xargs kill -9
```

#### Error: "Connection refused"
- Pastikan server sudah running
- Check port number yang benar
- Pastikan firewall tidak memblokir port

#### Compilation error
```bash
# Pastikan g++ terinstall
g++ --version

# Install jika belum ada (Ubuntu/Debian)
sudo apt-get install g++ build-essential
```

## 📝 Contoh Penggunaan

### TCP Echo Example
```cpp
// Server menerima pesan dan mengirim kembali
// Client: "Hello"
// Server: "Echo: Hello"
```

### UDP Example
```cpp
// Server menerima datagram dari client
// Tidak ada connection establishment
// Cocok untuk real-time data (gaming, streaming)
```

### JSON Message Example
```json
{
  "type": "message",
  "content": "Hello World",
  "timestamp": 1699999999
}
```

## 🎓 Learning Resources

### Recommended Reading
1. **Beej's Guide to Network Programming** (free online) - Excellent tutorial
2. **UNIX Network Programming** by W. Richard Stevens - Comprehensive reference
3. **C++ Socket Programming** tutorials on cppreference.com

### Online Tools
- `netcat` (nc) - Testing sockets
- `telnet` - Testing TCP connections
- Wireshark - Analyzing network packets
- `netstat`/`ss` - Viewing active connections

## 🤝 Contributing

Contributions are welcome! Silakan buat pull request atau buka issue untuk:
- Bug fixes
- Contoh baru
- Improvement dokumentasi
- Penjelasan lebih detail

## 📄 License

Project ini dibuat untuk tujuan edukasi. Feel free to use and modify.

## 👨‍🏫 Contact

Untuk pertanyaan atau diskusi lebih lanjut, silakan buka issue di repository ini.

---

**Happy Coding! 🚀**

*Project ini dibuat untuk sesi perkuliahan 150 menit tentang socket programming dan proses asinkron.*
