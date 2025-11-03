#include "../../includes/services/auth_service.h"

AuthService::AuthService(std::shared_ptr<XMLDataManager> data_manager,
                         std::shared_ptr<EncryptionUtil> encrypt_util)
    : data_manager_(data_manager), encrypt_util_(encrypt_util) {}

bool AuthService::Register(const std::map<std::string, std::string>& user_data) {
    std::string username = user_data.at("username");
    std::string password = user_data.at("password");
    std::string email = user_data.at("email");
    std::string phone = user_data.at("phone");
    std::string role = user_data.count("role") > 0 ? user_data.at("role") : "user";

    // 在 AuthService 中使用 EncryptionUtil 处理密码哈希
   // std::string hashed_password = encrypt_util_->HashPassword(password);
    User user("", username, password, email, phone, role);
    return data_manager_->SaveUser(user);
}

User* AuthService::Login(const std::string& username, const std::string& password) {
    User* user = data_manager_->GetUser(username);
    /*if (user && encrypt_util_->VerifyPassword(password, user->GetPassword())) {
        return user;
    }*/
    if (user && password==user->GetPassword()) {
        return user;
    }
    if (user) {
        delete user; // 如果密码不匹配，释放内存
    }
    return nullptr;
}

bool AuthService::ValidateUser(const std::string& username, const std::string& password) {
    User* user = data_manager_->GetUser(username);
    if (user) {
       // bool valid = encrypt_util_->VerifyPassword(password, user->GetPassword());
        bool valid = (password==user->GetPassword());
        delete user;
        return valid;
    }
    return false;
}
