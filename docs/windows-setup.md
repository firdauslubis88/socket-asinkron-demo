# Windows Setup Guide

Panduan setup environment untuk compile dan run program C++ socket di Windows.

## Prerequisites

### Option 1: MinGW (Recommended untuk beginners)

MinGW adalah port dari GNU Compiler Collection (GCC) untuk Windows.

#### Install MinGW

1. **Download MinGW-w64**
   - Website: https://www.mingw-w64.org/
   - Atau gunakan installer: https://github.com/niXman/mingw-builds-binaries/releases
   - Download versi `x86_64-win32-seh` (64-bit)

2. **Extract dan Add ke PATH**
   ```
   - Extract ke: C:\mingw64
   - Add ke PATH: C:\mingw64\bin
   ```

3. **Verify Installation**
   ```cmd
   g++ --version
   ```

   Output seharusnya menunjukkan versi g++.

#### Compile dengan MinGW

```cmd
# Single file
g++ -std=c++11 tcp_server.cpp -o tcp_server.exe -lws2_32

# With warnings
g++ -std=c++11 -Wall -Wextra tcp_server.cpp -o tcp_server.exe -lws2_32
```

**Important**: Flag `-lws2_32` untuk link dengan Winsock library!

### Option 2: Visual Studio (Microsoft Compiler)

#### Install Visual Studio

1. **Download Visual Studio Community** (gratis)
   - Website: https://visualstudio.microsoft.com/
   - Install workload: "Desktop development with C++"

2. **Compile dari Developer Command Prompt**
   ```cmd
   cl /EHsc tcp_server.cpp /link ws2_32.lib
   ```

### Option 3: CMake (Cross-platform build system)

#### Install CMake

1. **Download CMake**
   - Website: https://cmake.org/download/
   - Install versi Windows x64 Installer

2. **Verify Installation**
   ```cmd
   cmake --version
   ```

#### Build dengan CMake

```cmd
mkdir build
cd build
cmake ..
cmake --build .
```

## Quick Start - Compile Demo Programs

### Method 1: Using compile.bat

Buat file `compile.bat` di folder project:

```batch
@echo off
echo Compiling Socket Demo Programs...

REM Create bin directory if not exists
if not exist bin mkdir bin

REM TCP Examples
echo Compiling TCP Server...
g++ -std=c++11 -Wall -Iinclude src/tcp/tcp_server.cpp -o bin/tcp_server.exe -lws2_32

echo Compiling TCP Client...
g++ -std=c++11 -Wall -Iinclude src/tcp/tcp_client.cpp -o bin/tcp_client.exe -lws2_32

REM UDP Examples
echo Compiling UDP Server...
g++ -std=c++11 -Wall -Iinclude src/udp/udp_server.cpp -o bin/udp_server.exe -lws2_32

echo Compiling UDP Client...
g++ -std=c++11 -Wall -Iinclude src/udp/udp_client.cpp -o bin/udp_client.exe -lws2_32

echo Done!
pause
```

Jalankan:
```cmd
compile.bat
```

### Method 2: Manual Compilation

```cmd
# Compile TCP server
g++ -std=c++11 -Iinclude src/tcp/tcp_server.cpp -o bin/tcp_server.exe -lws2_32

# Compile TCP client
g++ -std=c++11 -Iinclude src/tcp/tcp_client.cpp -o bin/tcp_client.exe -lws2_32

# Run
bin\tcp_server.exe
bin\tcp_client.exe
```

## Common Issues

### Issue 1: "g++ is not recognized"

**Problem**: MinGW tidak ada di PATH

**Solution**:
1. Cek MinGW terinstall di `C:\mingw64\bin\g++.exe`
2. Add ke PATH:
   - Windows + R → `sysdm.cpl`
   - Advanced → Environment Variables
   - System variables → Path → Edit
   - Add: `C:\mingw64\bin`
3. Restart terminal/IDE

### Issue 2: "undefined reference to `WSAStartup@8`"

**Problem**: Lupa link dengan Winsock library

**Solution**: Add flag `-lws2_32` saat compile:
```cmd
g++ program.cpp -o program.exe -lws2_32
```

### Issue 3: Firewall Blocking Port

**Problem**: Windows Firewall memblokir port

**Solution**:
1. Windows Security → Firewall & network protection
2. Advanced settings → Inbound Rules → New Rule
3. Port → TCP → Specific port (misalnya 8080)
4. Allow the connection
5. Apply

Atau untuk testing, disable firewall sementara (tidak recommended untuk production).

### Issue 4: "Address already in use"

**Problem**: Port masih digunakan program lain atau belum di-release

**Solution**:

Check port usage:
```cmd
netstat -ano | findstr :8080
```

Kill process:
```cmd
taskkill /F /PID <process_id>
```

### Issue 5: Permission Denied for Port < 1024

**Problem**: Port 1-1023 adalah privileged ports

**Solution**: Gunakan port >= 1024 (misalnya 8080, 9000)

## Testing Connections

### Test TCP dengan telnet

```cmd
# Install telnet (jika belum ada)
dism /online /Enable-Feature /FeatureName:TelnetClient

# Test connection
telnet localhost 8080
```

### Test dengan netcat (nc)

Download netcat for Windows:
- https://nmap.org/ncat/

```cmd
# TCP test
nc localhost 8080

# UDP test
nc -u localhost 9000
```

### Monitor connections dengan netstat

```cmd
# Show all listening ports
netstat -an | findstr LISTENING

# Show specific port
netstat -an | findstr :8080
```

## IDE Setup

### Visual Studio Code

1. **Install VS Code**
   - Download: https://code.visualstudio.com/

2. **Install Extensions**
   - C/C++ (Microsoft)
   - C/C++ Extension Pack

3. **Configure tasks.json**

   `.vscode/tasks.json`:
   ```json
   {
       "version": "2.0.0",
       "tasks": [
           {
               "label": "build",
               "type": "shell",
               "command": "g++",
               "args": [
                   "-std=c++11",
                   "-Wall",
                   "-Iinclude",
                   "${file}",
                   "-o",
                   "${fileDirname}\\${fileBasenameNoExtension}.exe",
                   "-lws2_32"
               ],
               "group": {
                   "kind": "build",
                   "isDefault": true
               }
           }
       ]
   }
   ```

4. **Build dengan Ctrl+Shift+B**

### Visual Studio (Full IDE)

1. Create new project: Empty Project (C++)
2. Add source files
3. Project Properties:
   - C/C++ → General → Additional Include Directories: `include`
   - Linker → Input → Additional Dependencies: `ws2_32.lib`
4. Build: Ctrl+Shift+B

## Building All Examples

### Create build script

`build_all.bat`:
```batch
@echo off
setlocal enabledelayedexpansion

echo ================================
echo Socket Demo - Build All Examples
echo ================================
echo.

REM Create directories
if not exist bin mkdir bin

set CXXFLAGS=-std=c++11 -Wall -Wextra -Iinclude
set LDFLAGS=-lws2_32

echo [1/6] Building TCP Server...
g++ %CXXFLAGS% src\tcp\tcp_server.cpp -o bin\tcp_server.exe %LDFLAGS%
if errorlevel 1 goto :error

echo [2/6] Building TCP Client...
g++ %CXXFLAGS% src\tcp\tcp_client.cpp -o bin\tcp_client.exe %LDFLAGS%
if errorlevel 1 goto :error

echo [3/6] Building UDP Server...
g++ %CXXFLAGS% src\udp\udp_server.cpp -o bin\udp_server.exe %LDFLAGS%
if errorlevel 1 goto :error

echo [4/6] Building UDP Client...
g++ %CXXFLAGS% src\udp\udp_client.cpp -o bin\udp_client.exe %LDFLAGS%
if errorlevel 1 goto :error

echo [5/6] Building Async Server...
g++ %CXXFLAGS% src\async\async_tcp_server.cpp -o bin\async_server.exe %LDFLAGS%
if errorlevel 1 goto :error

echo [6/6] Building Async Client...
g++ %CXXFLAGS% src\async\async_tcp_client.cpp -o bin\async_client.exe %LDFLAGS%
if errorlevel 1 goto :error

echo.
echo ================================
echo Build completed successfully!
echo Binaries are in bin\ directory
echo ================================
goto :end

:error
echo.
echo ================================
echo Build failed!
echo ================================
exit /b 1

:end
pause
```

Run:
```cmd
build_all.bat
```

## Running Examples

### TCP Echo Demo

Terminal 1:
```cmd
cd bin
tcp_server.exe
```

Terminal 2:
```cmd
cd bin
tcp_client.exe
```

### UDP Demo

Terminal 1:
```cmd
cd bin
udp_server.exe
```

Terminal 2:
```cmd
cd bin
udp_client.exe
```

## Tips untuk Mahasiswa

1. **Gunakan MinGW untuk simplicity**
   - Lebih mudah setup
   - Compatible dengan Linux commands
   - Good for learning

2. **Test di localhost dulu**
   - Gunakan `127.0.0.1` atau `localhost`
   - Tidak perlu deal dengan network/firewall issues

3. **Gunakan port > 1024**
   - Port 8080, 9000, 3000 adalah good choices
   - Hindari port 80, 443, 22 (privileged)

4. **Enable compiler warnings**
   - `-Wall -Wextra` untuk catch common errors

5. **Use debugging output**
   - `std::cout` untuk track program flow
   - Print error codes: `WSAGetLastError()`

6. **Keep programs running in separate terminals**
   - Mudah untuk debug
   - Bisa lihat output dari server dan client

## Resources

- MinGW-w64: https://www.mingw-w64.org/
- Winsock Reference: https://docs.microsoft.com/en-us/windows/win32/winsock/
- CMake Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/
- VS Code C++ Setup: https://code.visualstudio.com/docs/cpp/config-mingw

---

**Happy Coding on Windows! 🪟**
