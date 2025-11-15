# VSCode Launch Configurations Guide

Panduan lengkap untuk menjalankan demo programs menggunakan VSCode (F5).

## Cara Menggunakan

1. Tekan **Ctrl+Shift+D** untuk membuka Debug panel
2. Pilih konfigurasi dari dropdown di bagian atas
3. Tekan **F5** untuk menjalankan
4. Tekan **Shift+F5** untuk stop semua instances

---

## Available Configurations

### 📌 TCP Demo (Basic)

#### TCP: Server + Client
- **What**: 1 TCP Server + 1 Client
- **Port**: 8080
- **Demo**: Basic TCP connection, echo server
- **Use**: Pengenalan dasar TCP socket programming

---

### 📌 UDP Demo (Connectionless)

#### UDP: Server + Client
- **What**: 1 UDP Server + 1 Client
- **Port**: 9000
- **Demo**: Connectionless protocol, datagram exchange
- **Use**: Memahami perbedaan TCP vs UDP

---

### 📌 Async Demo (Non-blocking I/O)

#### Async: Server + 1 Client (Debug)
- **What**: 1 Async Server + 1 Client
- **Port**: 8080
- **Demo**: select() dengan single client
- **Use**: Testing basic async functionality

#### Async: Server + 3 Clients (Debug) ⭐ RECOMMENDED
- **What**: 1 Async Server + 3 Clients simultaneously
- **Port**: 8080
- **Demo**: Multiple clients handled concurrently dengan select()
- **Use**: **Mendemonstrasikan kekuatan async I/O!**
- **What to observe**:
  - Server handle 3 clients bersamaan
  - Tidak ada blocking
  - Semua clients mendapat response

#### Async: Server + 1 Client (Release)
- Same as Debug, tapi versi Release (faster)

#### Async: Server + 3 Clients (Release) ⭐ RECOMMENDED
- Same as Debug, tapi versi Release (faster)

---

### 📌 Chat Demo (Multi-client + Threading)

#### Chat: Server + 1 Client (Debug)
- **What**: 1 Chat Server + 1 Client
- **Port**: 7777
- **Demo**: Basic chat functionality
- **Use**: Testing single user chat

#### Chat: Server + 2 Clients (Debug) ⭐ RECOMMENDED
- **What**: 1 Chat Server + 2 Clients simultaneously
- **Port**: 7777
- **Demo**: Real-time multi-user chat
- **Use**: **Demo broadcast messaging antar clients!**
- **What to observe**:
  - Client 1 kirim pesan → langsung muncul di Client 2
  - Join/leave notifications
  - Threading di client side (real-time receive)

#### Chat: Server + 1 Client (Release)
- Same as Debug, tapi versi Release

#### Chat: Server + 2 Clients (Release) ⭐ RECOMMENDED
- Same as Debug, tapi versi Release

---

### 📌 JSON Demo (Message Protocol)

#### JSON: Server + Client (Release)
- **What**: 1 JSON Server + 1 Client
- **Port**: 9999
- **Demo**: Structured message protocol dengan JSON
- **Use**: Understand professional message formatting
- **Commands**: `/time`, `/echo`, `/quit`

---

## Debug vs Release

### Debug Mode
- **Build**: `CMake: Build All (Debug)`
- **Binary**: `build/bin/Debug/*.exe`
- **Features**:
  - ✅ Bisa set breakpoints
  - ✅ Inspect variables
  - ✅ Step through code
- **Performance**: Slower (debug symbols included)
- **Use**: Development, learning, debugging

### Release Mode
- **Build**: `CMake: Build All (Release)`
- **Binary**: `build/bin/Release/*.exe`
- **Features**:
  - ❌ No breakpoints
  - ✅ Optimized code
  - ✅ Smaller binary
- **Performance**: Much faster
- **Use**: Demo presentasi, production-like testing

---

## Recommended Demo Sequence untuk Presentasi

### 1. Start Simple: TCP
```
Launch: "TCP: Server + Client"
```
- Show basic socket API
- Explain connection flow
- Demo echo functionality

### 2. Compare: UDP
```
Launch: "UDP: Server + Client"
```
- Show connectionless protocol
- Compare dengan TCP
- Explain use cases

### 3. Scale Up: Async
```
Launch: "Async: Server + 3 Clients (Release)"
```
- Show **3 windows** (1 server, 3 clients)
- **Demo**: Ketik pesan di Client 1, 2, 3 bersamaan
- **Observe**: Server handle semuanya tanpa blocking!
- Explain `select()` mechanism

### 4. Real Application: Chat
```
Launch: "Chat: Server + 2 Clients (Release)"
```
- Show **3 windows** (1 server, 2 clients)
- Login dengan username berbeda (Alice, Bob)
- **Demo**: Alice kirim pesan → langsung muncul di Bob
- Show join/leave notifications
- Explain threading di client

### 5. Professional: JSON
```
Launch: "JSON: Server + Client (Release)"
```
- Show structured messages
- Try commands: `/time`, `/echo`
- Explain JSON benefits

---

## Testing Multiple Clients

### Method 1: Use Compound Configurations (Recommended)
Pilih configuration yang sudah include multiple clients:
- `Async: Server + 3 Clients (Release)`
- `Chat: Server + 2 Clients (Release)`

### Method 2: Manual Launch
1. Launch server first:
   ```
   Select: "Run: Async Server (Release)"
   Press: F5
   ```
2. Add clients manually:
   - Klik icon "+" di Debug toolbar
   - Pilih "Run: Async Client (Release)"
   - Ulangi untuk client 2, 3, dst.

### Method 3: Terminal (untuk advanced users)
```bash
# Terminal 1 - Server
./build/bin/Release/async_server.exe

# Terminal 2, 3, 4 - Clients
./build/bin/Release/async_client.exe  # Client 1
./build/bin/Release/async_client.exe  # Client 2
./build/bin/Release/async_client.exe  # Client 3
```

---

## Troubleshooting

### "Port already in use"
**Cause**: Server dari run sebelumnya masih running
**Solution**:
- Tekan **Shift+F5** untuk stop all
- Atau tutup terminal window yang masih ada
- Wait 30 detik (TIME_WAIT state)

### "Connection refused"
**Cause**: Server belum running atau port salah
**Solution**:
- Pastikan server running dulu
- Check port number di server output
- Check firewall (Windows Defender)

### "Build failed"
**Cause**: Code belum di-compile
**Solution**:
1. Ctrl+Shift+P
2. "Tasks: Run Task"
3. "CMake: Build All (Release)"

### Multiple windows confusing
**Solution**:
- Arrange windows side-by-side
- Use different username untuk identify clients
- Watch server console untuk connection logs

---

## Tips untuk Demo Presentasi

### 1. Preparation
- Build semua targets: `CMake: Build All (Release)`
- Test semua configurations sebelum presentasi
- Close programs lain yang pakai port 8080, 7777, 9000, 9999

### 2. Screen Layout
- **2 monitors**: Server di monitor 1, clients di monitor 2
- **1 monitor**: Use Windows snap (Win+Arrow) untuk arrange windows

### 3. Demonstrate Clearly
- **Async demo**: Ketik di 3 clients bersamaan untuk show concurrent handling
- **Chat demo**: Gunakan username jelas (Alice, Bob) dan ketik pesan jelas
- **JSON demo**: Show JSON structure di console dengan jelas

### 4. Explain While Demoing
- Point out server logs showing connection/disconnection
- Highlight message flow (client → server → response)
- Explain what `select()` is doing behind the scenes

---

## Configuration File Location

File: `.vscode/launch.json`

Anda bisa edit manual jika ingin:
- Tambah more clients (e.g., 5 clients)
- Change port numbers
- Customize terminal settings

Example untuk 5 clients:
```json
{
    "name": "Async: Server + 5 Clients (Release)",
    "configurations": [
        "Run: Async Server (Release)",
        "Run: Async Client (Release)",
        "Run: Async Client (Release)",
        "Run: Async Client (Release)",
        "Run: Async Client (Release)",
        "Run: Async Client (Release)"
    ],
    "stopAll": true
}
```

---

**Created for**: Socket Programming Demo - Teknik Elektro UI
**Purpose**: VSCode launch configuration guide untuk presentasi dan testing
