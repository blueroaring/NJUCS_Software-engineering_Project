#include "../../includes/ui/register_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "../../includes/services/auth_service.h"
#include "../../includes/ui/main_window.h"  // 包含MainWindow头文件

// 修改构造函数
RegisterWindow::RegisterWindow(MainWindow* main_window, QWidget* parent)
    : QWidget(parent), main_window_(main_window) {
    SetupUI();
}

void RegisterWindow::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Title
    title_label_ = new QLabel("Online Shop - Register", this);
    title_label_->setAlignment(Qt::AlignCenter);
    QFont title_font = title_label_->font();
    title_font.setPointSize(16);
    title_font.setBold(true);
    title_label_->setFont(title_font);
    main_layout->addWidget(title_label_);

    // Form layout
    QFormLayout* form_layout = new QFormLayout();

    username_edit_ = new QLineEdit(this);
    username_edit_->setPlaceholderText("Choose a username");
    username_edit_->setMaxLength(20);
    form_layout->addRow("Username:", username_edit_);

    password_edit_ = new QLineEdit(this);
    password_edit_->setPlaceholderText("Enter your password");
    password_edit_->setEchoMode(QLineEdit::Password);
    form_layout->addRow("Password:", password_edit_);

    confirm_password_edit_ = new QLineEdit(this);
    confirm_password_edit_->setPlaceholderText("Confirm your password");
    confirm_password_edit_->setEchoMode(QLineEdit::Password);
    form_layout->addRow("Confirm Password:", confirm_password_edit_);

    email_edit_ = new QLineEdit(this);
    email_edit_->setPlaceholderText("your.email@example.com");
    form_layout->addRow("Email:", email_edit_);

    phone_edit_ = new QLineEdit(this);
    phone_edit_->setPlaceholderText("Your phone number");
    form_layout->addRow("Phone:", phone_edit_);

    // 添加管理员注册选项
    admin_checkbox_ = new QCheckBox("Register as Administrator", this);
    form_layout->addRow("", admin_checkbox_);

    // 管理员权限组（初始隐藏）
    admin_group_ = new QGroupBox("Administrator Registration", this);
    admin_group_->setVisible(false);

    QFormLayout* admin_layout = new QFormLayout(admin_group_);
    admin_password_edit_ = new QLineEdit(this);
    admin_password_edit_->setPlaceholderText("Enter administrator secret password");
    admin_password_edit_->setEchoMode(QLineEdit::Password);
    admin_layout->addRow("Admin Password:", admin_password_edit_);

    main_layout->addLayout(form_layout);
    main_layout->addWidget(admin_group_);

    // Buttons
    QHBoxLayout* button_layout = new QHBoxLayout();

    register_button_ = new QPushButton("Register", this);
    back_button_ = new QPushButton("Back to Login", this);

    button_layout->addWidget(register_button_);
    button_layout->addWidget(back_button_);

    main_layout->addLayout(button_layout);

    // Connect signals
    connect(register_button_, &QPushButton::clicked,
            this, &RegisterWindow::HandleRegister);
    connect(back_button_, &QPushButton::clicked,
            this, &RegisterWindow::HandleBackToLogin);
    connect(admin_checkbox_, &QCheckBox::stateChanged,
            this, &RegisterWindow::HandleAdminCheckboxChanged);

    // Set layout
    setLayout(main_layout);
}

void RegisterWindow::HandleRegister() {
    if (!ValidateInput()) {
        return;
    }

    std::map<std::string, std::string> user_data;
    user_data["username"] = username_edit_->text().toStdString();
    user_data["password"] = password_edit_->text().toStdString();
    user_data["email"] = email_edit_->text().toStdString();
    user_data["phone"] = phone_edit_->text().toStdString();

    // 根据复选框和密码验证设置角色
    std::string role = "user";
    if (admin_checkbox_->isChecked()) {
        // 检查管理员密码
        QString entered_admin_password = admin_password_edit_->text();
        if (entered_admin_password == QString::fromStdString(kAdminSecretPassword)) {
            role = "admin";
            user_data["role"] = "admin";
        } else {
            QMessageBox::warning(this, "Registration Failed",
                                 "Invalid administrator password.");
            return;
        }
    }

    // 使用MainWindow中的共享AuthService
    if (!main_window_ || !main_window_->GetAuthService()) {
        QMessageBox::warning(this, "System Error", "Authentication service not available.");
        return;
    }

    if (main_window_->GetAuthService()->Register(user_data)) {
        QString message = role == "admin" ?
                              "Administrator account created successfully!" :
                              "User account created successfully!";
        QMessageBox::information(this, "Registration Success", message);
        emit RegistrationSuccess();
    } else {
        QMessageBox::warning(this, "Registration Failed",
                             "Username already exists or registration failed.");
    }
}

void RegisterWindow::HandleBackToLogin() {
    emit ShowLogin();
}

void RegisterWindow::HandleAdminCheckboxChanged(int state) {
    // 根据复选框状态显示或隐藏管理员密码输入区域
    admin_group_->setVisible(state == Qt::Checked);

    // 调整窗口大小以适应内容变化
    adjustSize();
}

bool RegisterWindow::ValidateInput() {
    QString username = username_edit_->text();
    QString password = password_edit_->text();
    QString confirm_password = confirm_password_edit_->text();
    QString email = email_edit_->text();
    QString phone = phone_edit_->text();

    // Validate username
    if (username.isEmpty() || username.length() < 3) {
        QMessageBox::warning(this, "Validation Error",
                             "Username must be at least 3 characters long.");
        return false;
    }

    // Validate password
    if (password.length() < 6) {
        QMessageBox::warning(this, "Validation Error",
                             "Password must be at least 6 characters long.");
        return false;
    }

    // Check password confirmation
    if (password != confirm_password) {
        QMessageBox::warning(this, "Validation Error",
                             "Passwords do not match.");
        return false;
    }

    // Validate email format
    QRegularExpression email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!email_regex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please enter a valid email address.");
        return false;
    }

    // Validate phone (basic validation)
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please enter your phone number.");
        return false;
    }

    // 如果选择注册为管理员，验证管理员密码
    if (admin_checkbox_->isChecked()) {
        QString admin_password = admin_password_edit_->text();
        if (admin_password.isEmpty()) {
            QMessageBox::warning(this, "Validation Error",
                                 "Please enter the administrator password.");
            return false;
        }
    }

    return true;
}
