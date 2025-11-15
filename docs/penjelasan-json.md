# Penjelasan JSON Message Protocol

## Mengapa Menggunakan JSON?

Dalam komunikasi client-server, kita perlu **format yang terstruktur** untuk mengirim data. Ada beberapa pilihan:

### 1. Plain Text (Tidak Terstruktur)
```
Hello
GET_TIME
QUIT
```
**Masalah**: Sulit membedakan tipe pesan, tidak ada metadata, sulit di-parse.

### 2. Custom Protocol
```
MSG:Hello
CMD:GET_TIME
CMD:QUIT
```
**Masalah**: Harus buat parser sendiri, sulit extend, tidak standar.

### 3. JSON (Terstruktur & Standar) ✅
```json
{
  "type": "message",
  "content": "Hello",
  "timestamp": 1699999999
}
```
**Keuntungan**:
- ✅ Terstruktur dan mudah di-parse
- ✅ Standar industry (hampir semua bahasa support)
- ✅ Mudah di-extend (tambah field baru)
- ✅ Human-readable (mudah di-debug)

---

## Format JSON untuk Socket Communication

### Message Structure

```json
{
  "type": "message_type",
  "field1": "value1",
  "field2": "value2",
  ...
}
```

### Contoh Message Types

#### 1. Regular Message
```json
{
  "type": "message",
  "content": "Hello, World!",
  "timestamp": 1699999999
}
```

#### 2. Command Message
```json
{
  "type": "command",
  "command": "get_time",
  "args": "optional_argument"
}
```

#### 3. Response Message
```json
{
  "type": "response",
  "content": "Server time: 2024-11-15 12:34:56"
}
```

#### 4. Error Message
```json
{
  "type": "error",
  "content": "Unknown command: xyz"
}
```

---

## Demo JSON Protocol

### Arsitektur

```
Client                          Server
  |                               |
  | 1. Connect                    |
  |------------------------------>|
  |                               |
  | 2. Send JSON message          |
  |   {"type":"message",...}      |
  |------------------------------>|
  |                               | 3. Parse JSON
  |                               | 4. Process message
  | 5. Receive JSON response      |
  |   {"type":"response",...}     |
  |<------------------------------|
  |                               |
```

### Flow Komunikasi

1. **Client** membuat JSON message
   ```cpp
   std::string json = createSimpleJsonMessage("message", "Hello");
   // Result: {"type":"message","content":"Hello","timestamp":...}
   ```

2. **Client** mengirim ke server (dengan newline delimiter)
   ```cpp
   json += "\n";
   send(socket, json.c_str(), json.length(), 0);
   ```

3. **Server** menerima dan parse JSON
   ```cpp
   recv(socket, buffer, BUFFER_SIZE, 0);
   std::string type = extractJsonValue(buffer, "type");
   std::string content = extractJsonValue(buffer, "content");
   ```

4. **Server** membuat response JSON
   ```cpp
   std::string response = createSimpleJsonMessage("response", "Message received");
   ```

5. **Server** mengirim response
   ```cpp
   response += "\n";
   send(socket, response.c_str(), response.length(), 0);
   ```

---

## Helper Functions (json_helper.hpp)

### 1. Create JSON Message

```cpp
#include "json_helper.hpp"

// Simple message
std::string msg = createSimpleJsonMessage("message", "Hello");
// {"type":"message","content":"Hello","timestamp":1699999999}

// Chat message dengan sender
std::string chat = createChatJsonMessage("Alice", "Hi Bob!");
// {"type":"chat","sender":"Alice","content":"Hi Bob!"}

// Command message
std::string cmd = createCommandJsonMessage("get_users");
// {"type":"command","command":"get_users"}
```

### 2. Parse JSON Message

```cpp
std::string json = "{\"type\":\"message\",\"content\":\"Hello\"}";

// Extract nilai
std::string type = extractJsonValue(json, "type");        // "message"
std::string content = extractJsonValue(json, "content");  // "Hello"

// Check if key exists
bool hasType = jsonContainsKey(json, "type");  // true
```

---

## Implementasi di Demo

### Client Side

```cpp
// 1. User input
std::cout << "> ";
std::getline(std::cin, input);

// 2. Buat JSON message
std::string jsonMessage;
if (input == "/time") {
    jsonMessage = createCommandJsonMessage("time");
} else {
    jsonMessage = createSimpleJsonMessage("message", input);
}

// 3. Kirim
jsonMessage += "\n";
send(socket, jsonMessage.c_str(), jsonMessage.length(), 0);

// 4. Terima response
recv(socket, buffer, BUFFER_SIZE, 0);

// 5. Parse response
std::string responseType = extractJsonValue(buffer, "type");
std::string responseContent = extractJsonValue(buffer, "content");
std::cout << "Type: " << responseType << ", Content: " << responseContent;
```

### Server Side

```cpp
// 1. Terima JSON
recv(socket, buffer, BUFFER_SIZE, 0);

// 2. Parse JSON
std::string messageType = extractJsonValue(buffer, "type");

// 3. Handle berdasarkan type
std::string response;
if (messageType == "message") {
    std::string content = extractJsonValue(buffer, "content");
    response = createSimpleJsonMessage("response", "Got: " + content);

} else if (messageType == "command") {
    std::string cmd = extractJsonValue(buffer, "command");
    if (cmd == "time") {
        response = createSimpleJsonMessage("response", getCurrentTime());
    }
}

// 4. Kirim response
response += "\n";
send(socket, response.c_str(), response.length(), 0);
```

---

## Message Delimiter

**Masalah**: Bagaimana tahu kapan satu message selesai?

**Solusi**: Gunakan delimiter (newline `\n`)

```cpp
// Send dengan newline
std::string msg = "{\"type\":\"message\"}";
msg += "\n";  // Delimiter!
send(socket, msg.c_str(), msg.length(), 0);

// Receive dan remove newline
recv(socket, buffer, BUFFER_SIZE, 0);
std::string json(buffer);
if (json[json.length()-1] == '\n') {
    json = json.substr(0, json.length()-1);
}
```

**Alternatif Delimiter**:
- Newline (`\n`) - Paling sederhana
- Null byte (`\0`)
- Custom separator (e.g., `|||`)
- Length prefix (kirim panjang message dulu)

---

## Keuntungan JSON Protocol

### 1. Extensible
Mudah tambah field baru tanpa break compatibility:

```json
// Versi 1
{"type":"message","content":"Hi"}

// Versi 2 (tambah field timestamp)
{"type":"message","content":"Hi","timestamp":123}

// Client lama tetap bisa baca type & content
```

### 2. Type Safety
Bisa bedakan tipe data:

```json
{
  "id": 123,           // number
  "name": "Alice",     // string
  "online": true,      // boolean
  "scores": [1,2,3]    // array
}
```

### 3. Nested Data
Bisa kirim struktur kompleks:

```json
{
  "type": "user_info",
  "user": {
    "id": 123,
    "name": "Alice",
    "preferences": {
      "theme": "dark",
      "language": "en"
    }
  }
}
```

### 4. Debugging
Mudah lihat apa yang dikirim/diterima:

```
[SEND] {"type":"message","content":"Hello"}
[RECV] {"type":"response","content":"Message received"}
```

---

## Perbandingan: Plain Text vs JSON

| Aspek | Plain Text | JSON |
|-------|-----------|------|
| **Struktur** | Tidak ada | Terstruktur |
| **Parse** | Manual (error-prone) | Library tersedia |
| **Extend** | Sulit | Mudah (tambah field) |
| **Debug** | Sulit | Mudah (human-readable) |
| **Size** | Kecil | Lebih besar (overhead) |
| **Speed** | Cepat | Sedikit lebih lambat |

**Kesimpulan**: Untuk aplikasi kompleks, **JSON lebih baik**. Untuk protocol sangat simple, plain text cukup.

---

## Library JSON (Production-Ready)

Untuk project serius, gunakan library JSON yang proper:

### 1. nlohmann/json (Recommended untuk C++)

```cpp
#include "json.hpp"
using json = nlohmann::json;

// Create JSON
json msg;
msg["type"] = "message";
msg["content"] = "Hello";
msg["timestamp"] = std::time(nullptr);

// To string
std::string json_str = msg.dump();

// Parse
json parsed = json::parse(json_str);
std::string type = parsed["type"];
```

**Download**: https://github.com/nlohmann/json

### 2. RapidJSON (Fastest)

```cpp
#include "rapidjson/document.h"

Document doc;
doc.SetObject();
doc.AddMember("type", "message", doc.GetAllocator());
```

**Download**: https://rapidjson.org/

---

## Testing JSON Demo

### Test 1: Regular Message

```
Client> Hello World
[SEND] {"type":"message","content":"Hello World","timestamp":...}
[RECV] {"type":"response","content":"Message received: Hello World"}
```

### Test 2: Time Command

```
Client> /time
[SEND] {"type":"command","command":"time"}
[RECV] {"type":"response","content":"Server time: 2024-11-15 12:34:56"}
```

### Test 3: Echo Command

```
Client> /echo
[SEND] {"type":"command","command":"echo","args":"Hello from client!"}
[RECV] {"type":"response","content":"Echo: Hello from client!"}
```

---

## Best Practices

### 1. Selalu Validate JSON

```cpp
if (!jsonContainsKey(msg, "type")) {
    std::cerr << "Invalid JSON: missing 'type' field" << std::endl;
    return;
}
```

### 2. Handle Errors Gracefully

```cpp
try {
    json parsed = json::parse(json_str);
} catch (json::parse_error& e) {
    std::cerr << "JSON parse error: " << e.what() << std::endl;
}
```

### 3. Document Your Protocol

Buat dokumentasi tentang message format yang Anda gunakan:

```
# Message Types

## "message"
- content: string (required)
- timestamp: number (optional)

## "command"
- command: string (required)
- args: string (optional)
```

---

## Konsep yang Dipelajari

✅ **JSON** sebagai message format
✅ **Protocol design** untuk client-server
✅ **Message serialization/deserialization**
✅ **Structured data exchange**
✅ **Message delimiters**

---

**Dibuat untuk**: Demo Socket Programming - Teknik Elektro UI
**Tujuan**: Pengenalan praktis JSON protocol untuk komunikasi network
