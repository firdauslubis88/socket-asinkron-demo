# 🔄 FORCE RESET ke MSVC - Jika Masih Memilih MinGW

## Masalah:

VSCode CMake extension **masih memilih MinGW** walaupun sudah delete build folder dan select Visual Studio kit.

## Penyebab:

CMake extension menyimpan preference kit di workspace state VSCode. Ini tersimpan **di luar** project folder, jadi delete build folder saja tidak cukup.

---

## ✅ SOLUSI LENGKAP - FORCE RESET

### Step 1: Tutup VSCode Sepenuhnya

**PENTING**: Tutup semua window VSCode yang terbuka!

- Tutup VSCode
- Check Task Manager, pastikan tidak ada proses `Code.exe` yang masih running
- Jika ada, kill process tersebut

### Step 2: Delete Workspace State

CMake extension menyimpan kit preference di folder workspace state. Hapus folder ini:

**Windows**:
```cmd
# Delete VSCode workspace storage
rmdir /s /q "%APPDATA%\Code\User\workspaceStorage"
```

**ATAU** jika menggunakan VSCode Insiders:
```cmd
rmdir /s /q "%APPDATA%\Code - Insiders\User\workspaceStorage"
```

### Step 3: Delete Project Build Folder

```cmd
cd C:\repos\alpro\socket-asinkron-demo
rmdir /s /q build
```

### Step 4: Delete CMake Cache Files (jika ada)

```cmd
cd C:\repos\alpro\socket-asinkron-demo
del /s CMakeCache.txt
rmdir /s /q CMakeFiles
rmdir /s /q .cmake
```

### Step 5: Buka VSCode dari Developer Command Prompt

**HARUS dari Developer Command Prompt!**

```cmd
# Start Menu → "Developer Command Prompt for VS 2022"

cd C:\repos\alpro\socket-asinkron-demo
code .
```

### Step 6: Manual Select Kit (JANGAN Auto)

Ketika VSCode terbuka, **JANGAN** klik "Yes" jika ada popup "Would you like to configure this project?"

Instead, manually select kit:

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Select a Kit`
3. **Pilih dengan HATI-HATI**:
   - ✅ **Visual Studio Community 2022 Release - amd64**
   - ✅ **Visual Studio Community 2019 Release - amd64**
   - ✅ **Visual Studio Build Tools ... - amd64**

4. **JANGAN pilih**:
   - ❌ MinGW
   - ❌ GCC
   - ❌ [Unspecified]

### Step 7: Verify Kit Selection

Check status bar di bagian bawah VSCode. Harus terlihat:

```
[Kit: Visual Studio Community 2022 Release - amd64]
```

**TIDAK boleh**:
```
[Kit: GCC ...]
[Kit: MinGW ...]
```

Jika masih salah, **ULANGI** Step 6!

### Step 8: Configure dengan Kit yang Benar

1. Press `Ctrl+Shift+P`
2. Ketik: `CMake: Configure`
3. Tunggu proses selesai

**Expected output di OUTPUT panel (tab CMake)**:
```
[cmake] -- The CXX compiler identification is MSVC 19.xx.xxxxx
[cmake] -- Selecting Windows SDK version 10.0.xxxxx.0 to target Windows 10.0.xxxxx
[cmake] -- Check for working CXX compiler: ...cl.exe
[cmake] -- Build type: Debug
[cmake] -- C++ Compiler: MSVC
[cmake] -- Platform: Windows
[cmake] -- Configuring done
[cmake] -- Generating done
[cmake] -- Build files have been written to: .../build
```

**PENTING**: Harus ada tulisan **"MSVC"**, BUKAN "MinGW" atau "GCC"!

### Step 9: Verify Generator

Di OUTPUT panel, cari baris generator. Harus terlihat:

```
[cmake] -- Selecting Windows SDK version...
```

**TIDAK boleh**:
```
[cmake] -- The C compiler identification is GNU
[cmake] -- Using generator: MinGW Makefiles
```

### Step 10: Build

Jika semua benar:

1. Press `F7` atau `Ctrl+Shift+B`
2. Tunggu build selesai
3. Check output di `build\Debug\`

---

## 🔍 Alternative: Manual Kit File

Jika masih gagal, buat file `.vscode/cmake-kits.json` untuk force kit:

```json
[
  {
    "name": "Visual Studio Community 2022 Release - amd64",
    "visualStudio": "VisualStudio.17.0",
    "visualStudioArchitecture": "x64",
    "preferredGenerator": {
      "name": "Visual Studio 17 2022",
      "platform": "x64"
    }
  }
]
```

Save file ini, lalu:
1. Reload window: `Ctrl+Shift+P` → `Developer: Reload Window`
2. Select kit: `Ctrl+Shift+P` → `CMake: Select a Kit`
3. Pilih kit yang baru dibuat

---

## 🔎 Troubleshooting

### Masih memilih MinGW setelah semua step?

**Check ini**:

1. **Apakah VSCode dibuka dari Developer Command Prompt?**
   ```cmd
   # Coba run ini di terminal VSCode
   where cl
   ```
   Harus output path ke `cl.exe`, bukan "not found"!

2. **Apakah workspace storage sudah dihapus?**
   - Pastikan folder `%APPDATA%\Code\User\workspaceStorage` benar-benar terhapus

3. **Apakah kit benar-benar terselect?**
   - Check status bar di bawah
   - Press `Ctrl+Shift+P` → `CMake: Edit User-Local CMake Kits`
   - Verify kit Visual Studio ada di list

4. **Try clear CMake cache di VSCode**:
   - `Ctrl+Shift+P`
   - `CMake: Delete Cache and Reconfigure`

### Visual Studio Kit Tidak Muncul di List?

**Kemungkinan**:

1. Visual Studio belum terinstall dengan benar
2. Workload "Desktop development with C++" tidak terinstall

**Solusi**:

1. Buka **Visual Studio Installer**
2. Click **Modify** pada instalasi Visual Studio Anda
3. Pastikan **"Desktop development with C++"** tercentang
4. Install/Update
5. Restart komputer
6. Buka Developer Command Prompt
7. Verify `cl` command:
   ```cmd
   cl
   ```
   Should output: `Microsoft (R) C/C++ Optimizing Compiler ...`

---

## 📋 Checklist Final

- [ ] VSCode ditutup sepenuhnya (tidak ada proses Code.exe)
- [ ] Workspace storage dihapus (`%APPDATA%\Code\User\workspaceStorage`)
- [ ] Build folder dihapus (`build/`)
- [ ] CMake cache files dihapus
- [ ] VSCode dibuka dari **Developer Command Prompt for VS**
- [ ] Kit **Visual Studio** dipilih (BUKAN MinGW!)
- [ ] Status bar menunjukkan kit Visual Studio
- [ ] Configure output menunjukkan **"MSVC"**, bukan "MinGW"
- [ ] Build sukses
- [ ] Executables ada di `build\Debug\`

---

## 💡 Tips

**Cara tercepat detect apakah kit benar**:

Lihat OUTPUT panel (tab CMake) setelah configure. Baris pertama harus:

```
[cmake] -- The CXX compiler identification is MSVC
```

Jika baris pertama adalah:
```
[cmake] -- The CXX compiler identification is GNU
```

Artinya **MASIH SALAH**, masih pakai MinGW! Ulangi select kit!

---

Jika setelah semua ini masih gagal, **screenshot** OUTPUT panel (tab CMake) dan status bar, untuk troubleshooting lebih lanjut.
