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
│   └── examples/         # Contoh aplikasi lengkap (chat demo)
├── include/              # Header files
├── docs/                 # Dokumentasi tambahan
├── .vscode/              # VSCode workspace configuration
├── CMakeLists.txt        # CMake build configuration
├── build.bat             # Windows batch build script (alternative)
└── README.md             # File ini
```

## 🚀 Quick Start

### ⭐ Setup untuk MSVC (Visual Studio) - RECOMMENDED

Jika Anda menggunakan **Visual Studio** dengan compiler `cl.exe`:

**PENTING**: Sebelum memulai, pastikan Anda sudah buka VSCode dari **Developer Command Prompt for VS**!

1. **Buka Developer Command Prompt for VS 2022** (atau versi VS Anda)
   ```cmd
   # Start Menu → "Developer Command Prompt for VS 2022"
   ```

2. **Navigate ke project dan buka VSCode**
   ```cmd
   cd C:\path\to\socket-asinkron-demo
   code .
   ```

3. **Di VSCode, delete build folder jika ada** (untuk clear cache):
   - Hapus folder `build/` jika sudah ada
   - Atau run: `rmdir /s /q build`

4. **Select Visual Studio Kit**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Select a Kit`
   - Pilih: **Visual Studio Community 2022 Release - amd64** (atau versi VS Anda)
   - **JANGAN pilih** kit yang ada kata "MinGW"!

5. **Configure CMake**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Configure`
   - Tunggu sampai selesai tanpa error

6. **Build** (Press `F7` atau `Ctrl+Shift+B`)

7. **Run/Debug**:
   - Press `F5`
   - Pilih: "Debug: TCP Server (MSVC)" atau program lain

**Output**: Executables akan ada di `build\Debug\` (bukan `build\bin\`!)

**Jika ada error**: Lihat [SETUP_MSVC.md](docs/SETUP_MSVC.md) untuk troubleshooting lengkap.

---

### Prerequisites

#### Windows (Platform Utama)
- **Compiler**: MinGW g++ atau MSVC (Visual Studio)
- **Build System**: CMake 3.10 atau lebih baru
- **IDE**: **Visual Studio Code** (recommended) atau Visual Studio
- **OS**: Windows 10/11
- **Tools**: git (optional)
- **Winsock**: Sudah include di Windows

**VSCode Extensions** (required):
- C/C++ (Microsoft)
- CMake Tools (Microsoft)

Lihat [Windows Setup Guide](docs/windows-setup.md) untuk detailed installation instructions.

#### Linux/Unix (Optional)
- **Compiler**: g++ atau clang++ dengan support C++11
- **Build System**: CMake 3.10 atau lebih baru
- **OS**: Linux/Unix atau WSL
- **Tools**: git

### Instalasi

#### Method 1: VSCode + CMake (⭐ RECOMMENDED)

```cmd
# 1. Clone repository
git clone <repository-url>
cd socket-asinkron-demo

# 2. Buka di VSCode
code .

# 3. Install VSCode extensions (jika belum):
#    - C/C++ (Microsoft)
#    - CMake Tools (Microsoft)

# 4. Ketika VSCode terbuka:
#    - Press Ctrl+Shift+P
#    - Ketik: "CMake: Configure"
#    - Pilih compiler kit (MinGW atau MSVC)

# 5. Build:
#    - Press F7 atau Ctrl+Shift+B
#    - Atau klik "Build" button di status bar

# 6. Run:
#    - Press F5 (debug) atau Ctrl+F5 (run)
#    - Pilih target dari dropdown

# Executables ada di: build/bin/
```

#### Method 2: Command Line dengan CMake

```cmd
# Windows (MinGW)
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .

# Windows (Visual Studio / MSVC)
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Debug
# Executables ada di: build/Debug/

# Windows (Visual Studio 2022)
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Debug
# Executables ada di: build/Debug/

# Linux
mkdir build
cd build
cmake ..
cmake --build .
# Executables ada di: build/bin/
```

#### Method 3: Batch Script (Quick & Simple)

```cmd
# Build semua dengan batch script
build.bat

# Executables ada di: bin/
```

### Menjalankan Demo

#### Dari VSCode

1. **Configure** (first time): `Ctrl+Shift+P` → "CMake: Configure"
2. **Build**: `F7` atau `Ctrl+Shift+B`
3. **Run**: `F5` (debug) atau `Ctrl+F5` (run without debug)
4. Pilih program dari dropdown:
   - Debug: TCP Server
   - Debug: TCP Client
   - Debug: Chat Server
   - Debug: Chat Client

#### Dari Command Line

**Windows (MSVC / Visual Studio)**

Path executables: `build\Debug\` untuk Debug build, `build\Release\` untuk Release build

**TCP Echo Server**
```cmd
REM Terminal 1: Jalankan server
build\Debug\tcp_server.exe

REM Terminal 2: Jalankan client
build\Debug\tcp_client.exe

REM Atau gunakan telnet
telnet localhost 8080
```

**Chat Demo** ⭐ (Most Advanced)
```cmd
REM Terminal 1: Jalankan server
build\Debug\chat_server.exe

REM Terminal 2, 3, 4: Jalankan multiple clients
build\Debug\chat_client.exe
```

**Windows (MinGW)**

Path executables: `build\bin\`

**TCP Echo Server**
```cmd
REM Terminal 1: Jalankan server
build\bin\tcp_server.exe

REM Terminal 2: Jalankan client
build\bin\tcp_client.exe
```

**Linux**
```bash
# Sama seperti di atas, tapi tanpa .exe
./build/bin/tcp_server
./build/bin/tcp_client
# dst...
```

## 📖 Materi Pembelajaran

### 1. TCP Socket Programming
- Membuat TCP server dan client
- Connection-oriented communication
- Reliable data transfer
- Echo server implementation

**Files**: `src/tcp/tcp_server.cpp`, `src/tcp/tcp_client.cpp`

### 2. UDP Socket Programming
- Membuat UDP server dan client
- Connectionless communication
- Datagram-based transfer
- Perbandingan dengan TCP

**Files**: `src/udp/udp_server.cpp`, `src/udp/udp_client.cpp`

### 3. Asynchronous Processing
- Non-blocking sockets
- select() untuk multiple clients
- Concurrent connections handling
- Event-driven programming

**Files**: `src/async/async_tcp_server.cpp`, `src/async/async_tcp_client.cpp`

### 4. Chat Demo (Advanced)
- Multi-user chat application
- Broadcast messaging
- Username management
- Commands (/users, /quit)
- Join/leave notifications

**Files**: `src/examples/chat_demo/chat_server.cpp`, `src/examples/chat_demo/chat_client.cpp`

### 5. Utility Helpers
- JSON message helpers
- Socket utility functions
- Common patterns

**Files**: `src/utils/json_helper.hpp`, `src/utils/socket_utils.hpp`

## 🛠️ Build Commands

### VSCode (Recommended)

```
Keyboard Shortcuts:
- Ctrl+Shift+P → "CMake: Configure" (first time setup)
- F7 atau Ctrl+Shift+B → Build all targets
- Ctrl+Shift+P → "CMake: Build Target" → build specific target
- F5 → Debug with breakpoints
- Ctrl+F5 → Run without debugging

Available CMake Targets:
- tcp         → Build TCP server + client
- udp         → Build UDP server + client
- async       → Build async server + client
- chat        → Build chat server + client
- all_examples → Build semua programs

Individual Targets:
- tcp_server, tcp_client
- udp_server, udp_client
- async_server, async_client
- chat_server, chat_client

Tasks (Ctrl+Shift+P → "Tasks: Run Task"):
- CMake: Configure
- CMake: Build All
- CMake: Build TCP
- CMake: Build Chat
- Run: TCP Server
- Run: Chat Server
```

### CMake Command Line

```cmd
# First time configuration
cmake -S . -B build -G "MinGW Makefiles"

# Build all
cmake --build build

# Build specific target
cmake --build build --target tcp
cmake --build build --target chat
cmake --build build --target all_examples

# Clean
cmake --build build --target clean

# Rebuild from scratch
rm -rf build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

**Generator Options:**
- `"MinGW Makefiles"` - Untuk MinGW g++
- `"Visual Studio 16 2019"` - Untuk VS 2019
- `"Visual Studio 17 2022"` - Untuk VS 2022
- `"Ninja"` - Untuk Ninja build system

### Batch Script (Alternative)

```cmd
# Build all programs
build.bat

# Output: bin/tcp_server.exe, bin/tcp_client.exe, etc.
```

**Note**: CMake otomatis handle linking dengan `ws2_32.lib` (Winsock2) di Windows!

## 📚 Dokumentasi

Dokumentasi lengkap tersedia di folder `docs/`:
- **[Setup MSVC](docs/SETUP_MSVC.md)** ⭐ - Setup Visual Studio (MSVC) + VSCode + CMake
- **[Quick Start VSCode](docs/QUICK_START_VSCODE.md)** - Panduan cepat VSCode untuk pemula
- [Penjelasan TCP](docs/penjelasan-tcp.md) - Tutorial TCP programming
- [Penjelasan UDP](docs/penjelasan-udp.md) - Tutorial UDP programming
- [Penjelasan Async](docs/penjelasan-async.md) - Tutorial async/non-blocking
- [Windows Setup](docs/windows-setup.md) - Setup MinGW, VSCode, CMake (alternative)

## 🔍 Troubleshooting

### VSCode

#### Error: "CMake Tools extension not found"
Install extension dari VSCode marketplace:
- C/C++ (Microsoft)
- CMake Tools (Microsoft)

#### Error: "No CMake kits available"
1. Press `Ctrl+Shift+P`
2. Ketik: "CMake: Scan for Kits"
3. Pilih compiler yang terdeteksi (MinGW atau MSVC)

#### IntelliSense tidak working
1. Check `c_cpp_properties.json` sudah ada di `.vscode/`
2. Restart VSCode
3. Press `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database"

### CMake

#### Error: "CMake was unable to find a build program corresponding to MinGW Makefiles"

Ini terjadi ketika CMake cache masih menyimpan generator MinGW tapi Anda ingin pakai MSVC.

**Solusi**:
1. **Delete build folder**: `rmdir /s /q build` (atau hapus manual)
2. **Di VSCode**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Delete Cache and Reconfigure`
3. **Select Visual Studio Kit**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Select a Kit`
   - Pilih: **Visual Studio Community 2022 Release - amd64** (BUKAN MinGW!)
4. **Configure ulang**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Configure`

**Penting**: Pastikan VSCode dibuka dari **Developer Command Prompt for VS**!

#### Error: "CMake not found"
Install CMake dari: https://cmake.org/download/

Tambahkan ke PATH (Windows):
- `C:\Program Files\CMake\bin`

#### Error: "Generator not found"
Untuk MinGW:
```cmd
cmake .. -G "MinGW Makefiles"
```

Untuk Visual Studio:
```cmd
cmake .. -G "Visual Studio 16 2019"
# atau
cmake .. -G "Visual Studio 17 2022"
```

### Windows

#### Error: "g++ is not recognized"
MinGW belum terinstall atau belum di PATH.

**Solusi**: Lihat [Windows Setup Guide](docs/windows-setup.md)

#### Error: "undefined reference to WSAStartup"
CMake seharusnya otomatis link dengan ws2_32. Jika masih error:

**Manual fix**:
```cmake
target_link_libraries(target_name ws2_32)
```

#### Error: "Address already in use"
Port masih digunakan program lain.

**Solusi**:
```cmd
REM Check port usage
netstat -ano | findstr :8080

REM Kill process
taskkill /F /PID <process_id>
```

#### Firewall Blocking Port
Windows Firewall memblokir connection.

**Solusi**:
- Windows Security → Firewall → Advanced settings
- Inbound Rules → New Rule → Port (8080, 7777) → Allow

### Linux

#### Error: "Address already in use"
```bash
lsof -ti:8080 | xargs kill -9
```

#### Error: "cmake command not found"
```bash
# Ubuntu/Debian
sudo apt-get install cmake

# Fedora/RHEL
sudo dnf install cmake
```

## 📝 Program Examples

### 1. TCP Echo Server (Basic)
```
Port: 8080
Type: Connection-oriented
Features: Echo messages back to client
```

### 2. UDP Echo Server
```
Port: 9000
Type: Connectionless
Features: Echo datagrams
```

### 3. Async TCP Server
```
Port: 8080
Type: Multi-client (select())
Features: Handle up to 10 concurrent clients
```

### 4. Chat Server (Advanced) ⭐
```
Port: 7777
Type: Multi-user chat
Features:
- Username registration
- Broadcast messages to all users
- Join/leave notifications
- Commands: /users, /quit
- Timestamp tracking
```

## 📊 Project Statistics

```
Total Programs:     8 executables
Total Source Files: 10 files (8 .cpp + 2 .hpp)
Total Lines:        2,270+ lines of C++ code
Documentation:      5 markdown files
Build Systems:      CMake (primary), batch script (alternative)
IDE Support:        VSCode (fully configured)
```

## 🎓 Learning Resources

### Recommended Reading
1. **Beej's Guide to Network Programming** (free online) - Excellent tutorial
2. **UNIX Network Programming** by W. Richard Stevens - Comprehensive reference
3. **C++ Socket Programming** tutorials on cppreference.com
4. **CMake Tutorial** - https://cmake.org/cmake/help/latest/guide/tutorial/

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
*Dioptimalkan untuk development dengan VSCode + CMake di Windows.*
