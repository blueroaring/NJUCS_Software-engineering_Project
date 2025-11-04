#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <memory>
#include "../models/user.h"
#include "../services/auth_service.h"
#include "../services/product_service.h"
#include "../ui/login_window.h"
#include "../ui/register_window.h"
#include "../ui/product_list_widget.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

/**
 * @brief The main application window that manages different views.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    //used for other windows
    std::shared_ptr<AuthService> GetAuthService() { return auth_service_; }
    std::shared_ptr<ProductService> GetProductService() { return product_service_; }
    std::shared_ptr<XMLDataManager> GetDataManager() { return data_manager_; }

private slots:
    void HandleLoginSuccess(User* user);
    void HandleRegistrationSuccess();
    void HandleLogout();
    void ShowLoginView();
    void ShowRegisterView();
    void ShowProductListView();

private:
    void SetupUI();
    void SetupServices();

    QStackedWidget* stacked_widget_;
    LoginWindow* login_window_;
    RegisterWindow* register_window_;
    ProductListWidget* product_list_widget_;

    std::shared_ptr<XMLDataManager> data_manager_;
    std::shared_ptr<EncryptionUtil> encrypt_util_;
    std::shared_ptr<AuthService> auth_service_;
    std::shared_ptr<ProductService> product_service_;

    User* current_user_;
};

#endif  // MAIN_WINDOW_H
