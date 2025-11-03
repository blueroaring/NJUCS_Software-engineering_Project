#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
class QVBoxLayout;
class QFormLayout;
class QCheckBox;
class QGroupBox;
class MainWindow;
QT_END_NAMESPACE

/**
 * @brief Provides a user registration interface.
 */
class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    // 修改构造函数，接受MainWindow指针
    explicit RegisterWindow(MainWindow* main_window, QWidget* parent = nullptr);

signals:
    void RegistrationSuccess();
    void ShowLogin();

private slots:
    void HandleRegister();
    void HandleBackToLogin();
    void HandleAdminCheckboxChanged(int state);

private:
    void SetupUI();
    bool ValidateInput();

    MainWindow* main_window_;  // 添加MainWindow引用
    QLineEdit* username_edit_;
    QLineEdit* password_edit_;
    QLineEdit* confirm_password_edit_;
    QLineEdit* email_edit_;
    QLineEdit* phone_edit_;
    QCheckBox* admin_checkbox_;
    QLineEdit* admin_password_edit_;
    QGroupBox* admin_group_;
    QPushButton* register_button_;
    QPushButton* back_button_;
    QLabel* title_label_;

    const std::string kAdminSecretPassword = "admin123";
};

#endif  // REGISTER_WINDOW_H
