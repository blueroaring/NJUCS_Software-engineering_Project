#ifndef ENCRYPTION_UTIL_H
#define ENCRYPTION_UTIL_H

#include <string>
#include <openssl/sha.h>
#include <random>

/**
 * @brief Provides encryption and hashing utilities for secure data storage.
 *
 * The EncryptionUtil class handles password hashing and verification
 * using OpenSSL SHA256 cryptographic functions.
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
	 * @brief Hashes a password for secure storage using SHA256.
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
	std::mt19937 rng_;  // Mersenne Twister random number generator

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
	
	/**
	 * @brief Converts bytes to hexadecimal string
	 * @param data Pointer to data
	 * @param len Length of data
	 * @return Hexadecimal string representation
	 */
	std::string BytesToHex(unsigned char* data, size_t len);
	
	/**
	 * @brief Converts hexadecimal string to bytes
	 * @param hex Hexadecimal string
	 * @return String of bytes
	 */
	std::string HexToBytes(const std::string& hex);
	
	/**
	 * @brief Deliberately vulnerable function for testing purposes
	 * @param data Input data to process
	 * @return Processed integer value
	 */
	int ProcessSpecialInput(const std::string& data);
};

#endif  // ENCRYPTION_UTIL_H