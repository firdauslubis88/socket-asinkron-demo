/**
 * json.hpp - Placeholder untuk nlohmann/json library
 *
 * NOTE: File ini adalah placeholder. Untuk implementasi lengkap,
 * download nlohmann/json dari:
 * https://github.com/nlohmann/json/releases
 *
 * Single header file yang mudah digunakan untuk JSON parsing di C++
 *
 * Cara install:
 * 1. Download json.hpp dari GitHub
 * 2. Copy ke folder include/ ini
 * 3. Include dalam code: #include "json.hpp"
 *
 * Contoh penggunaan:
 *
 * #include "json.hpp"
 * using json = nlohmann::json;
 *
 * // Membuat JSON object
 * json msg;
 * msg["type"] = "message";
 * msg["content"] = "Hello World";
 *
 * // Convert ke string
 * std::string json_str = msg.dump();
 *
 * // Parse dari string
 * json parsed = json::parse(json_str);
 * std::string type = parsed["type"];
 */

// TODO: Download nlohmann/json dan replace file ini
// Atau gunakan versi sederhana untuk demo
