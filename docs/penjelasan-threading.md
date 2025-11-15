# Penjelasan Threading pada Chat Client

## Masalah yang Diselesaikan

### Sebelum Threading (Blocking Synchronous):

```
User mengetik input
    ↓
Tunggu user tekan Enter (STUCK DI SINI!)
    ↓
Kirim ke server
    ↓
Terima response
    ↓
Kembali ke "Tunggu user tekan Enter"
```

**Masalah**: Selama user belum mengetik apa-apa, program TIDAK BISA menerima pesan dari client lain!

### Setelah Threading (Concurrent):

```
THREAD 1 (Receive):              THREAD 2 (Main/Send):
  Loop forever:                    Loop forever:
    Tunggu pesan dari server         Tunggu input user
    Print ke screen                  Kirim ke server

  (Berjalan bersamaan!)
```

**Solusi**: Kedua proses berjalan **bersamaan** (concurrent), sehingga pesan langsung muncul!

---

## Konsep Threading Sederhana

### 1. Thread itu seperti "jalur eksekusi terpisah"

Bayangkan Anda punya 2 tangan:
- Tangan kiri: Terus-menerus cek HP untuk pesan masuk
- Tangan kanan: Menulis pesan untuk dikirim

Kedua tangan bekerja **bersamaan** (concurrent)!

### 2. Kode Threading di C++11

```cpp
#include <thread>   // Library untuk threading
#include <atomic>   // Untuk flag yang thread-safe

// Global flag untuk kontrol kedua thread
std::atomic<bool> g_running(true);

// Fungsi yang akan dijalankan di thread terpisah
void receiveMessages(SOCKET socket) {
    while (g_running) {
        // Terus-menerus listen pesan dari server
        recv(socket, buffer, BUFFER_SIZE, 0);
        std::cout << buffer;
    }
}

int main() {
    // Buat thread baru yang menjalankan receiveMessages()
    std::thread receiveThread(receiveMessages, clientSocket);

    // Main thread tetap di sini untuk user input
    while (g_running) {
        std::getline(std::cin, input);
        send(clientSocket, input.c_str(), input.length(), 0);
    }

    // Tunggu thread selesai sebelum exit
    receiveThread.join();
}
```

### 3. Komponen Penting

#### a. `std::thread`
Membuat thread baru yang menjalankan fungsi tertentu.

```cpp
std::thread myThread(functionName, arg1, arg2);
```

#### b. `std::atomic<bool>`
Variable boolean yang **thread-safe** (aman diakses dari multiple threads).

```cpp
std::atomic<bool> g_running(true);  // Bisa diakses dari thread mana saja

// Thread 1
while (g_running) { ... }

// Thread 2
g_running = false;  // Thread 1 akan berhenti
```

#### c. `thread.join()`
Tunggu sampai thread selesai sebelum melanjutkan.

```cpp
receiveThread.join();  // WAIT sampai receiveThread selesai
std::cout << "Thread sudah selesai";
```

---

## Mengapa Tidak Perlu Mutex di Sini?

**Mutex** diperlukan ketika multiple threads **menulis ke data yang sama**.

Di program chat client kita:
- **Thread 1 (Receive)**: Hanya **READ** dari socket, **WRITE** ke `std::cout`
- **Thread 2 (Send)**: Hanya **READ** dari `std::cin`, **WRITE** ke socket

Tidak ada shared data yang di-write oleh kedua thread, jadi **tidak perlu mutex**!

> **Note**: Di real-world app yang lebih kompleks, Anda perlu mutex untuk `std::cout` karena bisa terjadi "race condition" di output. Tapi untuk demo sederhana ini, kita skip untuk kesederhanaan.

---

## Alur Eksekusi Program

```
1. Main thread: Connect ke server
2. Main thread: Login dengan username
3. Main thread: START receive thread ← Thread baru dimulai!

   ┌─────────────────────────────────────────────┐
   │ CONCURRENT EXECUTION (Berjalan Bersamaan)   │
   ├─────────────────────────────────────────────┤
   │                                             │
   │ Receive Thread:        Main Thread:        │
   │   while (g_running)      while (g_running) │
   │     recv()                 getline()       │
   │     cout                   send()          │
   │                                             │
   └─────────────────────────────────────────────┘

4. User ketik "/quit"
5. Main thread: Set g_running = false
6. Main thread: join() ← Tunggu receive thread selesai
7. Cleanup dan exit
```

---

## Testing

### Test 1: Basic Messaging
1. Jalankan server: `chat_server.exe`
2. Jalankan 2 clients: `chat_client.exe` (2x)
3. Di Client 1: Ketik username "Alice"
4. Di Client 2: Ketik username "Bob"
5. **Jangan ketik apa-apa di Alice**, lalu ketik pesan di Bob
6. **Pesan dari Bob langsung muncul di Alice!** ✓

### Test 2: Join/Leave Notification
1. Jalankan Client 3 dengan username "Charlie"
2. **Notifikasi "Charlie joined" langsung muncul di Alice dan Bob!** ✓

---

## Perbandingan: Before vs After

| Aspek | Before (Blocking) | After (Threading) |
|-------|-------------------|-------------------|
| **Receive messages** | Hanya saat user kirim pesan | **Real-time!** |
| **User experience** | Harus ketik sesuatu dulu | Langsung terlihat |
| **Complexity** | Simple (1 thread) | Medium (2 threads) |
| **Concurrent** | ❌ No | ✅ Yes |

---

## Konsep yang Dipelajari

✅ **Threading**: Concurrent execution
✅ **std::thread**: Membuat thread baru
✅ **std::atomic**: Thread-safe variable
✅ **thread.join()**: Synchronization
✅ **Concurrent I/O**: Multiple operations bersamaan

---

## Catatan Penting

1. **Thread bukan magic**: Masih butuh design yang baik
2. **Race condition**: Bisa terjadi jika tidak hati-hati
3. **Mutex**: Diperlukan untuk shared data yang di-write multiple threads
4. **Debugging**: Lebih sulit dengan multiple threads
5. **Alternatif**: `select()`, `poll()`, `epoll()` (non-blocking I/O)

---

## Bacaan Lanjutan

- C++11 Threading: https://en.cppreference.com/w/cpp/thread
- Concurrency vs Parallelism
- Mutex dan Synchronization
- Non-blocking I/O dengan `select()`

---

**Dibuat untuk**: Demo Socket Programming - Teknik Elektro UI
**Tujuan**: Pengenalan praktis concurrent programming untuk mahasiswa tahun 2
