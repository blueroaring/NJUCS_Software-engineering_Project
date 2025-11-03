#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <string>
#include <map>
#include <memory>
#include "../models/user.h"
#include "../data/xml_data_manager.h"
#include "../services/encryption_util.h"

/**
 * @brief Handles user authentication and registration operations.
 *
 * The AuthService class provides methods for user registration, login,
 * and validation using the data manager and encryption utilities.
 */
class AuthService {
public:
    AuthService(std::shared_ptr<XMLDataManager> data_manager,
        std::shared_ptr<EncryptionUtil> encrypt_util);

    /**
     * @brief Registers a new user with the provided data.
     * @param user_data Map containing user registration data.
     * @return true if registration is successful, false otherwise.
     */
    bool Register(const std::map<std::string, std::string>& user_data);

    /**
     * @brief Authenticates a user with username and password.
     * @param username The username for login.
     * @param password The password for login.
     * @return Pointer to User object if login successful, nullptr otherwise.
     */
    User* Login(const std::string& username, const std::string& password);

    /**
     * @brief Validates user credentials.
     * @param username The username to validate.
     * @param password The password to validate.
     * @return true if credentials are valid, false otherwise.
     */
    bool ValidateUser(const std::string& username, const std::string& password);

private:
    std::shared_ptr<XMLDataManager> data_manager_;
    std::shared_ptr<EncryptionUtil> encrypt_util_;
};

#endif  // AUTH_SERVICE_H
