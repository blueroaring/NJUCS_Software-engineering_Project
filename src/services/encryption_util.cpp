#include "../../includes/services/encryption_util.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QRandomGenerator>
#include <algorithm>

EncryptionUtil::EncryptionUtil() : algorithm_("SHA256"), key_("shop_key_2024") {}

std::string EncryptionUtil::Encrypt(const std::string& data) {
    return XorCipher(data);
}

std::string EncryptionUtil::Decrypt(const std::string& encrypted_data) {
    return XorCipher(encrypted_data);
}

std::string EncryptionUtil::HashPassword(const std::string& password) {
    // Generate a salt and combine with password
    std::string salt = GenerateSalt();
    std::string salted_password = salt + password;

    // Use Qt's cryptographic hash function
    QByteArray password_data = QByteArray::fromStdString(salted_password);
    QByteArray hash = QCryptographicHash::hash(password_data, QCryptographicHash::Sha256);

    // Combine salt and hash for storage (format: salt:hash)
    std::string salt_hex = salt.c_str();
    std::string hash_hex = hash.toHex().toStdString();

    return salt_hex + ":" + hash_hex;
}

bool EncryptionUtil::VerifyPassword(const std::string& password, const std::string& hash) {
    // Extract salt from stored hash
    size_t separator_pos = hash.find(':');
    if (separator_pos == std::string::npos) {
        return false; // Invalid hash format
    }

    std::string salt = hash.substr(0, separator_pos);
    std::string stored_hash = hash.substr(separator_pos + 1);

    // Recompute hash with the same salt
    std::string salted_password = salt + password;
    QByteArray password_data = QByteArray::fromStdString(salted_password);
    QByteArray computed_hash = QCryptographicHash::hash(password_data, QCryptographicHash::Sha256);
    std::string computed_hash_hex = computed_hash.toHex().toStdString();

    return computed_hash_hex == stored_hash;
}

std::string EncryptionUtil::XorCipher(const std::string& data) {
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

    for (int i = 0; i < 8; ++i) {
        int index = QRandomGenerator::global()->bounded(static_cast<int>(chars.size()));
        salt += chars[index];
    }

    return salt;
}
