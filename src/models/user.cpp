#include "../../includes/models/user.h"

User::User() : user_id_(""), username_(""), password_(""),
    email_(""), phone_(""), role_("user") {}

User::User(const std::string& user_id, const std::string& username,
           const std::string& password, const std::string& email,
           const std::string& phone, const std::string& role)
    : user_id_(user_id), username_(username), password_(password),
    email_(email), phone_(phone), role_(role) {}

bool User::Register() {
    // Registration logic would be implemented in AuthService
    return true;
}

bool User::Login() {
    // Login logic would be implemented in AuthService
    return true;
}

bool User::IsAdmin() const {
    return role_ == "admin";
}
