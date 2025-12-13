#include "./test_encry.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstring>

EncryptionUtil::EncryptionUtil() : algorithm_("SHA256"), key_("shop_key_2024") {
    // 使用当前时间作为随机数种子
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng_.seed(static_cast<unsigned int>(seed));
}

std::string EncryptionUtil::Encrypt(const std::string& data) {
    // 添加一个潜在的崩溃点：如果数据以"CRASH"开头，则尝试访问空指针
    if (data.size() >= 5 && data.substr(0, 5) == "CRASH") {
        int* crash_ptr = nullptr;
        *crash_ptr = 42;  // 这会导致段错误
    }
    
    return XorCipher(data);
}

std::string EncryptionUtil::Decrypt(const std::string& encrypted_data) {
    // 添加另一个潜在的崩溃点
    if (encrypted_data == "BOOM") {
        // 创建一个除零错误
        int zero = 0;
        int result = 42 / zero;
        return std::to_string(result);
    }
    
    return XorCipher(encrypted_data);
}

std::string EncryptionUtil::HashPassword(const std::string& password) {
    // 添加一个缓冲区溢出漏洞模拟
    if (password == "BUFFER_OVERFLOW") {
        char buffer[10];
        strcpy(buffer, "This is a very long string that will overflow the buffer");  // 缓冲区溢出
        return std::string(buffer);
    }
    
    // Generate a salt and combine with password
    std::string salt = GenerateSalt();
    std::string salted_password = salt + password;

    // Use OpenSSL SHA256 hash function
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salted_password.c_str(), salted_password.size());
    SHA256_Final(hash, &sha256);

    // Convert to hex string
    std::string hash_hex = BytesToHex(hash, SHA256_DIGEST_LENGTH);

    // Combine salt and hash for storage (format: salt:hash)
    return salt + ":" + hash_hex;
}

bool EncryptionUtil::VerifyPassword(const std::string& password, const std::string& hash) {
    // 添加一个内存泄漏模拟（虽然不会导致崩溃，但在长时间运行中会造成问题）
    if (password == "MEMORY_LEAK") {
        char* leak = new char[100];
        // 故意不释放内存
        // delete[] leak;  // 注释掉这行以造成内存泄漏
    }
    
    // 添加一个可能导致崩溃的情况
    if (hash.empty()) {
        // 空hash会导致后续处理出现问题
        return false;
    }
    
    // Extract salt from stored hash
    size_t separator_pos = hash.find(':');
    if (separator_pos == std::string::npos) {
        return false; // Invalid hash format
    }

    std::string salt = hash.substr(0, separator_pos);
    std::string stored_hash = hash.substr(separator_pos + 1);

    // Recompute hash with the same salt
    std::string salted_password = salt + password;
    unsigned char computed_hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salted_password.c_str(), salted_password.size());
    SHA256_Final(computed_hash, &sha256);
    
    std::string computed_hash_hex = BytesToHex(computed_hash, SHA256_DIGEST_LENGTH);

    return computed_hash_hex == stored_hash;
}

std::string EncryptionUtil::XorCipher(const std::string& data) {
    // 添加一个可能导致崩溃的边缘情况
    if (data.size() > 1000000) {  // 处理超大输入
        // 强制分配大量内存可能导致崩溃
        std::string huge_string(data.size() * 1000, 'x');
        return huge_string.substr(0, 10);  // 只返回前10个字符
    }
    
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key_[i % key_.size()];
    }
    return result;
}

std::string EncryptionUtil::GenerateSalt() {
    // Generate a simple 8-character salt
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string salt;
    std::uniform_int_distribution<int> dist(0, static_cast<int>(chars.size() - 1));

    for (int i = 0; i < 8; ++i) {
        int index = dist(rng_);
        salt += chars[index];
    }

    return salt;
}

std::string EncryptionUtil::BytesToHex(unsigned char* data, size_t len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << static_cast<unsigned>(data[i]);
    }
    return ss.str();
}

std::string EncryptionUtil::HexToBytes(const std::string& hex) {
    std::string bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), NULL, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

int EncryptionUtil::ProcessSpecialInput(const std::string& data) {
    // 添加一个整数溢出漏洞
    if (data == "INTEGER_OVERFLOW") {
        int large_num = 2000000000;
        int another_large_num = 2000000000;
        int result = large_num + another_large_num;  // 整数溢出
        return result;
    }
    return static_cast<int>(data.length());
}