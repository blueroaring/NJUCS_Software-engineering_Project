#ifndef USER_H
#define USER_H

#include <string>

/**
 * @brief Represents a user in the online shop system.
 *
 * The User class encapsulates user information and provides methods
 * for user authentication and role management.
 */
class User {
public:
    User();
    User(const std::string& user_id, const std::string& username,
        const std::string& password, const std::string& email,
        const std::string& phone, const std::string& role);

    // Getters
    std::string GetUserId() const { return user_id_; }
    std::string GetUsername() const { return username_; }
    std::string GetPassword() const { return password_; }
    std::string GetEmail() const { return email_; }
    std::string GetPhone() const { return phone_; }
    std::string GetRole() const { return role_; }

    // Setters
    void SetUserId(const std::string& user_id) { user_id_ = user_id; }
    void SetUsername(const std::string& username) { username_ = username; }
    void SetPassword(const std::string& password) { password_ = password; }
    void SetEmail(const std::string& email) { email_ = email; }
    void SetPhone(const std::string& phone) { phone_ = phone; }
    void SetRole(const std::string& role) { role_ = role; }

    /**
     * @brief Registers a new user in the system.
     * @return true if registration is successful, false otherwise.
     */
    bool Register();

    /**
     * @brief Authenticates the user with the system.
     * @return true if login is successful, false otherwise.
     */
    bool Login();

    /**
     * @brief Checks if the user has administrator privileges.
     * @return true if user is admin, false otherwise.
     */
    bool IsAdmin() const;

private:
    std::string user_id_;
    std::string username_;
    std::string password_;
    std::string email_;
    std::string phone_;
    std::string role_;
};

#endif  // USER_H