# Penjelasan TCP (Transmission Control Protocol)

## Apa itu TCP?

TCP adalah protokol komunikasi yang **connection-oriented** dan **reliable**. Artinya:
- Ada proses "handshake" sebelum data dikirim
- Data dijamin sampai dengan urutan yang benar
- Ada konfirmasi penerimaan data

## Analogi Sederhana

TCP seperti **menelepon teman**:
1. Anda dial nomor telepon (connect)
2. Teman mengangkat telepon (accept)
3. Kalian berbicara bolak-balik (send/receive)
4. Kalian tutup telepon (close)

## Karakteristik TCP

### ✅ Kelebihan
- **Reliable**: Data dijamin sampai
- **Ordered**: Data sampai dengan urutan yang benar
- **Error checking**: Deteksi dan koreksi error otomatis
- **Flow control**: Pengaturan kecepatan pengiriman data

### ❌ Kekurangan
- **Overhead**: Lebih lambat karena ada proses handshake dan acknowledgment
- **Lebih kompleks**: Lebih banyak resource yang dibutuhkan
- **Connection required**: Harus establish connection dulu

## Flow TCP Connection

```
Client                          Server
  |                               |
  |--- SYN ---------------------->|  (1. Client inisiasi)
  |<-- SYN-ACK -------------------|  (2. Server konfirmasi)
  |--- ACK ---------------------->|  (3. Connection established)
  |                               |
  |--- Data -------------------->|  (4. Transfer data)
  |<-- ACK -----------------------|  (5. Konfirmasi terima)
  |                               |
  |--- FIN ---------------------->|  (6. Tutup connection)
  |<-- FIN-ACK -------------------|  (7. Konfirmasi tutup)
  |                               |
```

## Langkah-langkah Programming TCP

### Server Side
```cpp
1. socket()    // Buat socket
2. bind()      // Bind ke IP dan port
3. listen()    // Dengarkan koneksi masuk
4. accept()    // Terima koneksi dari client
5. recv()      // Terima data
6. send()      // Kirim data
7. close()     // Tutup connection
```

### Client Side
```cpp
1. socket()    // Buat socket
2. connect()   // Connect ke server
3. send()      // Kirim data
4. recv()      // Terima data
5. close()     // Tutup connection
```

## Contoh Use Cases

TCP cocok untuk:
- 📧 **Email** (data harus lengkap)
- 🌐 **Web browsing** (HTML harus lengkap)
- 📁 **File transfer** (file tidak boleh corrupt)
- 💬 **Chat applications** (pesan harus sampai)
- 🔐 **SSH/Telnet** (command harus tepat)

## Perbedaan dengan UDP

| Aspek | TCP | UDP |
|-------|-----|-----|
| Connection | Ya, perlu establish | Tidak perlu |
| Reliability | Reliable | Best effort |
| Order | Terurut | Tidak dijamin |
| Speed | Lebih lambat | Lebih cepat |
| Overhead | Tinggi | Rendah |
| Use case | File transfer, web | Gaming, streaming |

## Error Handling Common

### "Address already in use"
Server port masih digunakan oleh process lain atau belum di-release OS.

**Solusi**: Set `SO_REUSEADDR` option

### "Connection refused"
Server tidak running atau firewall memblokir.

**Solusi**: Pastikan server running dan port terbuka

### "Broken pipe"
Client menutup connection saat server masih mengirim data.

**Solusi**: Check connection status sebelum send

## Tips Programming

1. **Always check return values**
   ```cpp
   if (send(sock, data, len, 0) < 0) {
       perror("send failed");
       return -1;
   }
   ```

2. **Null-terminate strings**
   ```cpp
   buffer[bytes_read] = '\0';
   ```

3. **Close sockets properly**
   ```cpp
   close(socket_fd);
   ```

4. **Handle partial sends**
   ```cpp
   // send() mungkin tidak mengirim semua data sekaligus
   while (total_sent < len) {
       sent = send(sock, data + total_sent, len - total_sent, 0);
       if (sent < 0) break;
       total_sent += sent;
   }
   ```

## Port Numbers

- **0-1023**: Well-known ports (HTTP: 80, HTTPS: 443, SSH: 22)
- **1024-49151**: Registered ports
- **49152-65535**: Dynamic/private ports

Untuk demo, gunakan port > 1024 (misalnya 8080, 9000).

## Resources Tambahan

- Beej's Guide to Network Programming
- man pages: `man 2 socket`, `man 2 bind`, `man 2 listen`
- RFC 793: TCP Specification
