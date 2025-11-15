# 🔧 FIX: CMake MinGW Makefiles Error dengan MSVC

## Error yang Anda alami:

```
[cmake] CMake Error: CMake was unable to find a build program corresponding to "MinGW Makefiles".
CMAKE_MAKE_PROGRAM is not set. You probably need to select a different build tool.
[cmake] CMake Error: CMAKE_CXX_COMPILER not set, after EnableLanguage
```

## Penyebab:

CMake masih menggunakan cache generator "MinGW Makefiles" dari konfigurasi sebelumnya, padahal Anda ingin menggunakan Visual Studio (MSVC).

## ✅ Solusi Lengkap (Ikuti Step by Step):

### Step 1: Pastikan VSCode Tertutup

Tutup VSCode terlebih dahulu jika masih terbuka.

### Step 2: Delete Build Folder

Dari command prompt, navigate ke project folder:

```cmd
cd C:\repos\alpro\socket-asinkron-demo
rmdir /s /q build
```

Atau hapus manual folder `build/` di Windows Explorer.

### Step 3: Buka VSCode dari Developer Command Prompt

**PENTING**: Harus dari Developer Command Prompt!

```cmd
# Buka Developer Command Prompt for VS 2022
# Start Menu → "Developer Command Prompt for VS 2022"

# Navigate ke project
cd C:\repos\alpro\socket-asinkron-demo

# Launch VSCode
code .
```

### Step 4: Delete CMake Cache di VSCode

Ketika VSCode sudah terbuka:

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Delete Cache and Reconfigure`
3. Tunggu selesai (mungkin error, tidak masalah)

### Step 5: Select Visual Studio Kit

**INI STEP PALING PENTING!**

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Select a Kit`
3. **PILIH yang ada kata "Visual Studio"**, contoh:
   - ✅ `Visual Studio Community 2022 Release - amd64`
   - ✅ `Visual Studio Community 2019 Release - amd64`
   - ✅ `Visual Studio Build Tools 2022 Release - amd64`

4. **JANGAN pilih** yang ada kata:
   - ❌ MinGW
   - ❌ GCC
   - ❌ Clang (kecuali Clang dari Visual Studio)

### Step 6: Scan for Kits (jika Visual Studio tidak muncul)

Jika tidak ada kit Visual Studio di list:

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Scan for Kits`
3. Tunggu proses scan selesai
4. Ulangi Step 5

### Step 7: Configure CMake

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Configure`
3. Tunggu proses selesai

**Expected output di OUTPUT panel (CMake):**
```
[cmake] -- The CXX compiler identification is MSVC ...
[cmake] -- Detecting CXX compiler ABI info
[cmake] -- Detecting CXX compiler ABI info - done
[cmake] -- Build type: Debug
[cmake] -- C++ Compiler: MSVC
[cmake] -- Platform: Windows
[cmake] -- Socket Library: ws2_32 (Winsock2)
[cmake] -- Configuring done
[cmake] -- Generating done
```

**TIDAK boleh ada tulisan "MinGW Makefiles"!**

### Step 8: Build

Jika configure sukses:

1. Press `F7` atau `Ctrl+Shift+B`
2. Tunggu build selesai

**Output executables akan ada di**: `build\Debug\`

### Step 9: Verify

Check apakah executable ada:

```cmd
dir build\Debug
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

### Step 10: Run/Debug

Press `F5` dan pilih:
- Debug: TCP Server (MSVC)
- Debug: Chat Server (MSVC)
- dll.

---

## 🔍 Troubleshooting Tambahan

### Masih error "MinGW Makefiles"?

Coba manual delete CMake cache files:

```cmd
cd C:\repos\alpro\socket-asinkron-demo
del /s CMakeCache.txt
rmdir /s /q CMakeFiles
rmdir /s /q build
```

Kemudian restart VSCode dan ulangi dari Step 3.

### Error: "cl is not recognized"?

Anda belum buka VSCode dari Developer Command Prompt.

**Solusi**:
1. Tutup VSCode
2. Buka **Developer Command Prompt for VS 2022** dari Start Menu
3. `cd` ke project folder
4. `code .` untuk launch VSCode
5. Ulangi dari Step 4

### Kit Visual Studio tidak muncul?

**Kemungkinan**:
1. Visual Studio belum terinstall dengan benar
2. Workload "Desktop development with C++" belum diinstall

**Solusi**:
1. Buka Visual Studio Installer
2. Modify installation
3. Pastikan **"Desktop development with C++"** workload tercentang
4. Install/Update
5. Restart, kemudian ulangi dari Step 6

---

## ✅ Checklist Akhir

Pastikan:
- [ ] Build folder sudah dihapus
- [ ] VSCode dibuka dari Developer Command Prompt for VS
- [ ] Kit yang dipilih adalah **Visual Studio**, bukan MinGW
- [ ] Configure sukses tanpa error "MinGW Makefiles"
- [ ] Build sukses (F7)
- [ ] Executables ada di `build\Debug\`

---

## 📞 Jika Masih Bermasalah

Capture screenshot dari:
1. OUTPUT panel (pilih "CMake" dari dropdown)
2. Status bar di VSCode (bagian bawah, yang menunjukkan kit dan build type)

Dan share untuk troubleshooting lebih lanjut.

---

**Good luck!** 🚀
