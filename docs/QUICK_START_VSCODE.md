# Quick Start Guide - VSCode Setup

Panduan cepat untuk setup project ini di VSCode untuk pertama kali.

## 📋 Prerequisites Check

Sebelum mulai, pastikan sudah install:

### 1. MinGW (Compiler)

**Check apakah sudah terinstall:**
```cmd
g++ --version
```

Jika muncul error "g++ is not recognized":

**Install MinGW:**
1. Download dari: https://github.com/niXman/mingw-builds-binaries/releases
2. Pilih: `x86_64-13.2.0-release-win32-seh-msvcrt-rt_v11-rev1.7z`
3. Extract ke: `C:\mingw64`
4. Add ke PATH:
   - Windows + R → `sysdm.cpl`
   - Advanced → Environment Variables
   - System variables → Path → Edit → New
   - Add: `C:\mingw64\bin`
5. Restart terminal/VSCode
6. Test lagi: `g++ --version`

### 2. CMake

**Check apakah sudah terinstall:**
```cmd
cmake --version
```

Jika error:

**Install CMake:**
1. Download dari: https://cmake.org/download/
2. Pilih: "Windows x64 Installer"
3. Install dengan option: "Add CMake to PATH"
4. Restart terminal/VSCode
5. Test: `cmake --version`

### 3. VSCode Extensions

Buka VSCode, press `Ctrl+Shift+X`, install:
- **C/C++** (by Microsoft)
- **CMake Tools** (by Microsoft)

## 🚀 Step-by-Step Setup

### Step 1: Open Project di VSCode

```cmd
# Buka folder project
cd path/to/socket-asinkron-demo
code .
```

### Step 2: Select Compiler Kit

1. **Tunggu VSCode load** (status bar bawah akan muncul notifikasi CMake)

2. **Jika muncul popup "Select a Kit"**:
   - Pilih: `GCC x.x.x x86_64-w64-mingw32` (yang ada MinGW)
   - **JANGAN pilih** yang Visual Studio kecuali Anda install VS

3. **Jika tidak muncul popup**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Select a Kit`
   - Pilih: `GCC x.x.x x86_64-w64-mingw32`

4. **Jika tidak ada kit sama sekali**:
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Scan for Kits`
   - Tunggu selesai
   - Pilih: `GCC x.x.x x86_64-w64-mingw32`

### Step 3: Configure Project

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Configure`
3. Tunggu proses selesai (lihat OUTPUT panel di bawah)

**Expected output:**
```
[cmake] -- Build type: Debug
[cmake] -- C++ Compiler: GCC
[cmake] -- Platform: Windows
[cmake] -- Socket Library: ws2_32 (Winsock2)
[cmake] -- Configuring done
[cmake] -- Generating done
```

**Jika error**, lihat section Troubleshooting di bawah.

### Step 4: Build

1. Press `F7` atau `Ctrl+Shift+B`
2. Atau klik "Build" di status bar bawah

**Expected output:**
```
[build] Building folder: socket-asinkron-demo
[build] [1/8] Building CXX object ...
[build] [2/8] Building CXX object ...
...
[build] [8/8] Linking CXX executable tcp_server.exe
[build] Build finished
```

**Check hasil build:**
```cmd
dir build\bin
```

Should see:
```
tcp_server.exe
tcp_client.exe
udp_server.exe
udp_client.exe
async_server.exe
async_client.exe
chat_server.exe
chat_client.exe
```

### Step 5: Run!

**Method 1: From VSCode (Debug)**
1. Press `F5`
2. Pilih: "Debug: TCP Server" (atau program lain)
3. Program akan run di terminal

**Method 2: From Terminal**
```cmd
# TCP Example
build\bin\tcp_server.exe
# Di terminal lain:
build\bin\tcp_client.exe
```

## ❌ Common Errors & Solutions

### Error 1: "No CMake kits available"

**Artinya:** VSCode tidak detect compiler

**Solusi:**
1. Pastikan MinGW sudah terinstall: `g++ --version`
2. Restart VSCode
3. `Ctrl+Shift+P` → `CMake: Scan for Kits`
4. `Ctrl+Shift+P` → `CMake: Select a Kit` → Pilih GCC

### Error 2: "CMake executable not found"

**Artinya:** CMake belum terinstall atau tidak di PATH

**Solusi:**
1. Install CMake dari cmake.org
2. Add to PATH
3. Restart VSCode

### Error 3: "Generator 'MinGW Makefiles' not found"

**Artinya:** MinGW tidak di PATH atau belum terinstall

**Solusi:**
1. Check: `mingw32-make --version`
2. Jika error, add `C:\mingw64\bin` ke PATH
3. Restart VSCode
4. `Ctrl+Shift+P` → `CMake: Delete Cache and Reconfigure`

### Error 4: Pilih Visual Studio tapi tidak punya VS

**Artinya:** Salah pilih compiler kit

**Solusi:**
1. `Ctrl+Shift+P` → `CMake: Select a Kit`
2. Pilih yang ada kata "GCC" atau "MinGW"
3. **JANGAN** pilih "Visual Studio" jika tidak install VS
4. `Ctrl+Shift+P` → `CMake: Configure`

### Error 5: "undefined reference to WSAStartup"

**Artinya:** Tidak link dengan ws2_32 (Winsock2 library)

**Solusi:**
CMake seharusnya otomatis handle ini. Jika masih error:
1. Check CMakeLists.txt ada `target_link_libraries(... ws2_32)`
2. Rebuild: `Ctrl+Shift+P` → `CMake: Clean Rebuild`

### Error 6: IntelliSense error (red squiggles) tapi build success

**Artinya:** IntelliSense belum configure

**Solusi:**
1. Check file `.vscode/c_cpp_properties.json` ada
2. `Ctrl+Shift+P` → `C/C++: Reset IntelliSense Database`
3. Restart VSCode

## 🎯 Alternative: Batch Script (Jika VSCode Ribet)

Kalau VSCode setup terlalu complicated, gunakan batch script:

```cmd
# Simple build
build.bat

# Run
bin\tcp_server.exe
# Di terminal lain:
bin\tcp_client.exe
```

**Note:** Batch script tetap butuh MinGW g++ terinstall dan di PATH.

## 📸 Visual Guide

### Good Kit Selection
```
✅ GCC 13.2.0 x86_64-w64-mingw32
✅ GCC 11.2.0 x86_64-w64-mingw32
```

### Bad Kit Selection (if you don't have Visual Studio)
```
❌ Visual Studio Community 2019 Release - amd64
❌ Visual Studio Community 2022 Release - amd64
```

## 🆘 Still Having Issues?

1. **Check OUTPUT panel** di VSCode (View → Output → pilih "CMake/Build")
2. **Copy error message** yang muncul
3. **Cek versi tools**:
   ```cmd
   g++ --version
   cmake --version
   mingw32-make --version
   ```

4. **Try manual CMake** di command line:
   ```cmd
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles"
   cmake --build .
   ```

5. Jika manual CMake works, issue ada di VSCode configuration. Coba:
   - Restart VSCode
   - Delete `.vscode/` folder dan buka lagi
   - Reinstall CMake Tools extension

## 📝 Checklist

Sebelum build, pastikan:
- [ ] MinGW g++ terinstall (`g++ --version` works)
- [ ] CMake terinstall (`cmake --version` works)
- [ ] MinGW di PATH (`mingw32-make --version` works)
- [ ] VSCode extensions installed (C/C++, CMake Tools)
- [ ] Selected correct kit (GCC/MinGW, bukan Visual Studio)
- [ ] CMake configured successfully (no errors in OUTPUT)

Jika semua ✅, harusnya build berhasil!
