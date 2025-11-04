#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QWidget>
#include <memory>
#include "../models/user.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class MainWindow;
QT_END_NAMESPACE

/**
 * @brief Provides a login interface for user authentication.
 */
class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(MainWindow* main_window, QWidget* parent = nullptr);

signals:
    void LoginSuccess(User* user);
    void ShowRegister();

private slots:
    void HandleLogin();
    void HandleRegister();

private:
    void SetupUI();

    MainWindow* main_window_;
    QLineEdit* username_edit_;
    QLineEdit* password_edit_;
    QPushButton* login_button_;
    QPushButton* register_button_;
    QLabel* title_label_;
};

#endif  // LOGIN_WINDOW_H
