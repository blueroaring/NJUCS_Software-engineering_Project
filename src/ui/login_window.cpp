#include "../../includes/ui/login_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include "../../includes/services/auth_service.h"
#include "../../includes/ui/main_window.h"  // 包含MainWindow头文件

// 修改构造函数
LoginWindow::LoginWindow(MainWindow* main_window, QWidget* parent)
    : QWidget(parent), main_window_(main_window) {
    SetupUI();
}

void LoginWindow::SetupUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Title
    title_label_ = new QLabel("Online Shop - Login", this);
    title_label_->setAlignment(Qt::AlignCenter);
    QFont title_font = title_label_->font();
    title_font.setPointSize(16);
    title_font.setBold(true);
    title_label_->setFont(title_font);
    main_layout->addWidget(title_label_);

    // Form layout
    QFormLayout* form_layout = new QFormLayout();

    username_edit_ = new QLineEdit(this);
    username_edit_->setPlaceholderText("Enter your username");
    form_layout->addRow("Username:", username_edit_);

    password_edit_ = new QLineEdit(this);
    password_edit_->setPlaceholderText("Enter your password");
    password_edit_->setEchoMode(QLineEdit::Password);
    form_layout->addRow("Password:", password_edit_);

    main_layout->addLayout(form_layout);

    // Buttons
    QHBoxLayout* button_layout = new QHBoxLayout();

    login_button_ = new QPushButton("Login", this);
    register_button_ = new QPushButton("Register", this);

    button_layout->addWidget(login_button_);
    button_layout->addWidget(register_button_);

    main_layout->addLayout(button_layout);

    // Connect signals
    connect(login_button_, &QPushButton::clicked, this, &LoginWindow::HandleLogin);
    connect(register_button_, &QPushButton::clicked, this, &LoginWindow::HandleRegister);

    // Set layout
    setLayout(main_layout);
}

void LoginWindow::HandleLogin() {
    std::string username = username_edit_->text().toStdString();
    std::string password = password_edit_->text().toStdString();

    if (username.empty() || password.empty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    // 使用MainWindow中的共享AuthService
    if (!main_window_ || !main_window_->GetAuthService()) {
        QMessageBox::warning(this, "System Error", "Authentication service not available.");
        return;
    }

    User* user = main_window_->GetAuthService()->Login(username, password);
    if (user) {
        emit LoginSuccess(user);
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void LoginWindow::HandleRegister() {
    emit ShowRegister();
}
