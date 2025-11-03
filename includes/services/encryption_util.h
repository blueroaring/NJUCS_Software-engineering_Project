#ifndef ENCRYPTION_UTIL_H
#define ENCRYPTION_UTIL_H

#include <string>

/**
 * @brief Provides encryption and hashing utilities for secure data storage.
 *
 * The EncryptionUtil class handles password hashing and verification
 * using Qt's built-in cryptographic functions.
 */
class EncryptionUtil {
public:
	EncryptionUtil();

	/**
	 * @brief Encrypts data using a simple XOR algorithm (for demonstration).
	 * @param data The data to encrypt.
	 * @return Encrypted string.
	 */
	std::string Encrypt(const std::string& data);

	/**
	 * @brief Decrypts data using XOR algorithm.
	 * @param encrypted_data The data to decrypt.
	 * @return Decrypted string.
	 */
	std::string Decrypt(const std::string& encrypted_data);

	/**
	 * @brief Hashes a password for secure storage using QCryptographicHash.
	 * @param password The password to hash.
	 * @return Hashed password string in hex format.
	 */
	std::string HashPassword(const std::string& password);

	/**
	 * @brief Verifies a password against a hash.
	 * @param password The password to verify.
	 * @param hash The hash to compare against.
	 * @return true if password matches hash, false otherwise.
	 */
	bool VerifyPassword(const std::string& password, const std::string& hash);

private:
	std::string algorithm_;
	std::string key_;

	/**
	 * @brief Simple XOR encryption/decryption implementation.
	 * @param data The data to process.
	 * @return Processed string.
	 */
	std::string XorCipher(const std::string& data);

	/**
	 * @brief Generates a simple salt for password hashing.
	 * @return Salt string.
	 */
	std::string GenerateSalt();
};

#endif  // ENCRYPTION_UTIL_H