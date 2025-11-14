/**
 * json_helper.hpp - Simple JSON Helper Functions
 *
 * File ini menyediakan helper functions sederhana untuk:
 * - Create simple JSON messages
 * - Parse simple JSON messages (manual, bukan full parser)
 *
 * CATATAN PENTING:
 * Ini adalah SIMPLIFIED version untuk educational purposes.
 * Untuk production code, gunakan nlohmann/json library yang proper.
 *
 * Download nlohmann/json dari:
 * https://github.com/nlohmann/json/releases
 *
 * Copy single header file ke include/json.hpp
 *
 * Usage dengan nlohmann/json (recommended):
 *   #include "json.hpp"
 *   using json = nlohmann::json;
 *
 *   json msg;
 *   msg["type"] = "message";
 *   msg["content"] = "Hello";
 *   std::string json_str = msg.dump();
 */

#ifndef JSON_HELPER_HPP
#define JSON_HELPER_HPP

#include <string>
#include <sstream>
#include <ctime>

/**
 * Create simple JSON message (manual construction)
 *
 * Format: {"type":"<type>","content":"<content>","timestamp":<time>}
 *
 * Example:
 *   std::string msg = createSimpleJsonMessage("chat", "Hello World");
 *   // Result: {"type":"chat","content":"Hello World","timestamp":1699999999}
 */
inline std::string createSimpleJsonMessage(const std::string& type, const std::string& content) {
    std::ostringstream oss;
    oss << "{"
        << "\"type\":\"" << type << "\","
        << "\"content\":\"" << content << "\","
        << "\"timestamp\":" << std::time(nullptr)
        << "}";
    return oss.str();
}

/**
 * Create JSON message with sender info
 *
 * Format: {"type":"<type>","sender":"<sender>","content":"<content>"}
 */
inline std::string createChatJsonMessage(const std::string& sender, const std::string& content) {
    std::ostringstream oss;
    oss << "{"
        << "\"type\":\"chat\","
        << "\"sender\":\"" << sender << "\","
        << "\"content\":\"" << content << "\""
        << "}";
    return oss.str();
}

/**
 * Create JSON command message
 *
 * Format: {"type":"command","command":"<cmd>","args":"<args>"}
 */
inline std::string createCommandJsonMessage(const std::string& command, const std::string& args = "") {
    std::ostringstream oss;
    oss << "{"
        << "\"type\":\"command\","
        << "\"command\":\"" << command << "\"";
    if (!args.empty()) {
        oss << ",\"args\":\"" << args << "\"";
    }
    oss << "}";
    return oss.str();
}

/**
 * Extract value dari simple JSON string (VERY BASIC PARSER!)
 *
 * CATATAN: Ini adalah parser SANGAT sederhana untuk demo.
 * Hanya bekerja untuk JSON simple tanpa nested objects.
 *
 * Example:
 *   std::string json = "{\"type\":\"chat\",\"content\":\"Hello\"}";
 *   std::string type = extractJsonValue(json, "type");
 *   // Result: "chat"
 */
inline std::string extractJsonValue(const std::string& json, const std::string& key) {
    // Find key position
    std::string searchKey = "\"" + key + "\":\"";
    size_t keyPos = json.find(searchKey);

    if (keyPos == std::string::npos) {
        // Try without quotes (for numbers)
        searchKey = "\"" + key + "\":";
        keyPos = json.find(searchKey);

        if (keyPos == std::string::npos) {
            return "";  // Key not found
        }

        // Extract number value
        size_t valueStart = keyPos + searchKey.length();
        size_t valueEnd = json.find_first_of(",}", valueStart);

        if (valueEnd == std::string::npos) {
            return "";
        }

        return json.substr(valueStart, valueEnd - valueStart);
    }

    // Extract string value
    size_t valueStart = keyPos + searchKey.length();
    size_t valueEnd = json.find("\"", valueStart);

    if (valueEnd == std::string::npos) {
        return "";  // Malformed JSON
    }

    return json.substr(valueStart, valueEnd - valueStart);
}

/**
 * Check if JSON contains a key
 */
inline bool jsonContainsKey(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":";
    return json.find(searchKey) != std::string::npos;
}

/**
 * Example usage:
 *
 * // Create JSON messages
 * std::string msg1 = createSimpleJsonMessage("ping", "");
 * std::string msg2 = createChatJsonMessage("Alice", "Hello Bob!");
 * std::string msg3 = createCommandJsonMessage("get_users");
 *
 * // Parse JSON (simple)
 * std::string json = "{\"type\":\"chat\",\"sender\":\"Alice\",\"content\":\"Hi\"}";
 * std::string type = extractJsonValue(json, "type");        // "chat"
 * std::string sender = extractJsonValue(json, "sender");    // "Alice"
 * std::string content = extractJsonValue(json, "content");  // "Hi"
 *
 * // Check for keys
 * bool hasType = jsonContainsKey(json, "type");  // true
 */

// Note untuk mahasiswa:
// ======================
// Ini adalah implementasi SEDERHANA untuk pembelajaran.
// Di real projects, WAJIB gunakan JSON library yang proper seperti:
//
// 1. nlohmann/json (C++, paling populer)
//    - Single header file
//    - Modern C++ API
//    - Very easy to use
//
// 2. RapidJSON (C++, very fast)
//    - Header-only
//    - Focus on performance
//
// 3. jsoncpp (C++, mature)
//    - Widely used
//    - Good documentation
//
// Kenapa tidak manual parsing?
// - Error prone (typo, escaping, nested objects, dll)
// - Sulit maintain
// - Performance issue
// - Security vulnerabilities (injection, buffer overflow)
//
// Gunakan library yang sudah battle-tested!

#endif // JSON_HELPER_HPP
