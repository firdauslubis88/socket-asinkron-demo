# Setup untuk MSVC (Visual Studio Compiler)

Panduan khusus jika Anda menggunakan MSVC (`cl.exe`) dari Visual Studio.

## ✅ Yang Sudah Anda Punya

- [x] Visual Studio atau Visual Studio Build Tools
- [x] `cl.exe` bisa dijalankan dari terminal
- [x] VSCode dengan CMake Tools extension

## 🚀 Setup VSCode dengan MSVC

### Step 1: Buka Project

```cmd
cd path\to\socket-asinkron-demo
code .
```

### Step 2: Select Compiler Kit (MSVC)

Ketika VSCode terbuka:

1. **Jika muncul popup "Select a Kit":**
   - Pilih yang ada kata **"Visual Studio"**, misalnya:
     - `Visual Studio Community 2022 Release - amd64`
     - `Visual Studio Community 2019 Release - amd64`
     - `Visual Studio Build Tools ... - amd64`

2. **Jika tidak muncul:**
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Select a Kit`
   - Pilih kit Visual Studio yang tersedia

3. **Jika tidak ada kit Visual Studio:**
   - Press `Ctrl+Shift+P`
   - Ketik: `CMake: Scan for Kits`
   - Tunggu selesai
   - Ulangi step 2

### Step 3: Configure

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Configure`
3. Tunggu proses selesai

**Expected output di OUTPUT panel:**
```
[cmake] -- Build type: Debug
[cmake] -- C++ Compiler: MSVC
[cmake] -- Platform: Windows
[cmake] -- Socket Library: ws2_32 (Winsock2)
[cmake] -- Configuring done
[cmake] -- Generating done
```

### Step 4: Build

Press `F7` atau `Ctrl+Shift+B`

**Expected output:**
```
[build] Building folder: socket-asinkron-demo
[build] Microsoft (R) C/C++ Optimizing Compiler ...
[build] tcp_server.cpp
[build] tcp_client.cpp
...
[build] Build finished
```

### Step 5: Check Output

```cmd
dir build\bin\Debug
```

Atau:

```cmd
dir build\Debug
```

Should see:
```
tcp_server.exe
tcp_client.exe
udp_server.exe
...
```

**Note:** MSVC output biasanya di `build/Debug/` atau `build/Release/`, bukan `build/bin/`.

### Step 6: Run

**From Terminal:**
```cmd
# Check dimana executable-nya
dir build\Debug\tcp_server.exe

# Jika ada, run:
build\Debug\tcp_server.exe

# Di terminal lain:
build\Debug\tcp_client.exe
```

**From VSCode:**
- Press `F5`
- Pilih: "Debug: TCP Server"

## ⚙️ Generator yang Digunakan

Dengan MSVC, CMake akan otomatis gunakan generator Visual Studio, misalnya:
- `Visual Studio 17 2022` (untuk VS 2022)
- `Visual Studio 16 2019` (untuk VS 2019)
- `Visual Studio 15 2017` (untuk VS 2017)

**Bukan** `MinGW Makefiles`!

## 📁 Output Directory Differences

### MinGW:
```
build/
└── bin/
    ├── tcp_server.exe
    └── tcp_client.exe
```

### MSVC:
```
build/
├── Debug/
│   ├── tcp_server.exe
│   └── tcp_client.exe
└── Release/
    ├── tcp_server.exe
    └── tcp_client.exe
```

CMake otomatis handle ini untuk Anda!

## 🐛 Common Issues dengan MSVC

### Error 1: "Cannot find cl.exe"

**Solusi:** Buka **Developer Command Prompt for VS** atau **x64 Native Tools Command Prompt**

Atau dari PowerShell/CMD biasa, jalankan:
```cmd
# For VS 2022
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

# For VS 2019
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

Kemudian buka VSCode dari terminal tersebut:
```cmd
code path\to\socket-asinkron-demo
```

### Error 2: Kit tidak terdeteksi

**Solusi:**
1. Pastikan Visual Studio terinstall lengkap (bukan hanya Build Tools)
2. `Ctrl+Shift+P` → `CMake: Scan for Kits`
3. Restart VSCode

### Error 3: Build berhasil tapi executable tidak ada di build/bin/

**Expected behavior dengan MSVC!**

Executable ada di:
- `build/Debug/` untuk Debug build
- `build/Release/` untuk Release build

**Cara run:**
```cmd
build\Debug\tcp_server.exe
```

### Error 4: "LNK1104: cannot open file 'ws2_32.lib'"

**Solusi:**
- Biasanya tidak terjadi jika setup benar
- Pastikan Visual Studio terinstall dengan "Desktop development with C++"
- Reinstall Visual Studio dengan workload tersebut

## 🎯 Quick Command Reference

```cmd
# Configure
Ctrl+Shift+P → CMake: Configure

# Build
F7 atau Ctrl+Shift+B

# Run (Debug build)
build\Debug\tcp_server.exe
build\Debug\tcp_client.exe

# Build Release
Ctrl+Shift+P → CMake: Set Build Type → Release
F7

# Run (Release build)
build\Release\tcp_server.exe
```

## 🔧 Manual CMake (if VSCode issues)

Dari **Developer Command Prompt**:

```cmd
mkdir build
cd build

# Auto-detect Visual Studio
cmake ..

# Atau specify version explicitly
cmake .. -G "Visual Studio 17 2022"

# Build Debug
cmake --build . --config Debug

# Build Release
cmake --build . --config Release

# Run
Debug\tcp_server.exe
```

## ⚙️ IntelliSense Configuration (Optional)

VSCode akan otomatis detect compiler dari CMake Tools. Tapi jika IntelliSense error:

1. Press `Ctrl+Shift+P`
2. Ketik: `C/C++: Select a Configuration`
3. Pilih: **"Win32 (MSVC)"**

File `.vscode/c_cpp_properties.json` sudah dikonfigurasi untuk MSVC dengan:
- Compiler: `cl.exe`
- IntelliSense Mode: `windows-msvc-x64`
- C++ Standard: `c++11`

## ✅ Verification Checklist

Pastikan:
- [ ] Visual Studio atau Build Tools terinstall
- [ ] `cl` command works di terminal
- [ ] CMake Tools extension installed di VSCode
- [ ] Selected Visual Studio kit (not MinGW!)
- [ ] CMake configured without errors
- [ ] Built successfully (F7)
- [ ] Executables ada di `build/Debug/`
- [ ] IntelliSense configuration: "Win32 (MSVC)" (optional)

## 🆚 MSVC vs MinGW

| Aspect | MSVC | MinGW |
|--------|------|-------|
| Compiler | `cl.exe` | `g++.exe` |
| Generator | Visual Studio | MinGW Makefiles |
| Output Dir | build/Debug/ | build/bin/ |
| Debugger | MSVC debugger | GDB |
| Setup | Lebih complex | Lebih simple |
| Performance | Optimized for Windows | Cross-platform |
| Binary Size | Larger | Smaller |

Keduanya **sama-sama bagus** untuk project ini!

## 📝 Recommended: Use Developer Command Prompt

Untuk avoid path issues, selalu run VSCode dari **Developer Command Prompt**:

1. Start Menu → "Developer Command Prompt for VS 2022"
2. Navigate ke project:
   ```cmd
   cd C:\path\to\socket-asinkron-demo
   ```
3. Launch VSCode:
   ```cmd
   code .
   ```

Ini ensure semua MSVC tools ada di PATH.
