#include "../../includes/ui/main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), current_user_(nullptr) {
    SetupServices();
    SetupUI();
    ShowLoginView();
}

MainWindow::~MainWindow() {
    delete current_user_;
}

void MainWindow::SetupServices() {
    // 获取应用程序目录并构建资源文件路径
    QString app_dir = QApplication::applicationDirPath();
    QString resources_dir = app_dir + "/resources";
    QString users_file = resources_dir + "/users.xml";
    QString products_file = resources_dir + "/products.xml";

    qDebug() << "Setting up services with files:";
    qDebug() << "Users:" << users_file;
    qDebug() << "Products:" << products_file;

    data_manager_ = std::make_shared<XMLDataManager>(
        users_file.toStdString(),
        products_file.toStdString());
    encrypt_util_ = std::make_shared<EncryptionUtil>();
    auth_service_ = std::make_shared<AuthService>(data_manager_, encrypt_util_);
    product_service_ = std::make_shared<ProductService>(data_manager_);
}

void MainWindow::SetupUI() {
    setWindowTitle("Online Shop System");
    setMinimumSize(800, 600);

    // Create central widget and stacked layout
    QWidget* central_widget = new QWidget(this);
    stacked_widget_ = new QStackedWidget(central_widget);

    // 使用新的构造函数，传递this(MainWindow)指针
    login_window_ = new LoginWindow(this, stacked_widget_);
    register_window_ = new RegisterWindow(this, stacked_widget_);
    product_list_widget_ = new ProductListWidget(this, stacked_widget_);

    // Add views to stacked widget
    stacked_widget_->addWidget(login_window_);
    stacked_widget_->addWidget(register_window_);
    stacked_widget_->addWidget(product_list_widget_);

    // Connect signals
    connect(login_window_, &LoginWindow::LoginSuccess,
            this, &MainWindow::HandleLoginSuccess);
    connect(login_window_, &LoginWindow::ShowRegister,
            this, &MainWindow::ShowRegisterView);

    connect(register_window_, &RegisterWindow::RegistrationSuccess,
            this, &MainWindow::HandleRegistrationSuccess);
    connect(register_window_, &RegisterWindow::ShowLogin,
            this, &MainWindow::ShowLoginView);

    connect(product_list_widget_, &ProductListWidget::LogoutRequested,
            this, &MainWindow::HandleLogout);

    // Set up layout
    QVBoxLayout* main_layout = new QVBoxLayout(central_widget);
    main_layout->addWidget(stacked_widget_);
    setCentralWidget(central_widget);
}

void MainWindow::HandleLoginSuccess(User* user) {
    current_user_ = user;
    product_list_widget_->SetCurrentUser(user);
    product_list_widget_->LoadProducts();
    ShowProductListView();

    // 在状态栏显示用户信息和角色
    QString role_display = user->IsAdmin() ? "Administrator" : "User";
    QString welcome_message = user->IsAdmin() ?
                                  QString("Welcome, Administrator %1!").arg(QString::fromStdString(user->GetUsername())) :
                                  QString("Welcome, %1!").arg(QString::fromStdString(user->GetUsername()));

    statusBar()->showMessage(welcome_message);

    // 如果是管理员，显示特殊提示
    if (user->IsAdmin()) {
        QMessageBox::information(this, "Admin Access",
                                 "You have logged in as administrator. You can manage all products in the system.");
    }
}

void MainWindow::HandleRegistrationSuccess() {
    ShowLoginView();
    QMessageBox::information(this, "Registration Successful",
                             "Your account has been created successfully. Please login.");
}

void MainWindow::HandleLogout() {
    delete current_user_;
    current_user_ = nullptr;
    ShowLoginView();
    statusBar()->clearMessage();
}

void MainWindow::ShowLoginView() {
    stacked_widget_->setCurrentWidget(login_window_);
}

void MainWindow::ShowRegisterView() {
    stacked_widget_->setCurrentWidget(register_window_);
}

void MainWindow::ShowProductListView() {
    stacked_widget_->setCurrentWidget(product_list_widget_);
}
