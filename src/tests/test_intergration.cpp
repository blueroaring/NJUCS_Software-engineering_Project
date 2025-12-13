#include "../../includes/tests/test_intergration.h"
#include <QMap>
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QDebug>

void TestIntegration::initTestCase()
{
    qDebug() << "Initializing Integration test case...";

    // 创建临时目录用于测试文件
    QString tempUsersFile = QDir::tempPath() + "/integration_test_users_" + QString::number(QCoreApplication::applicationPid()) + ".xml";
    QString tempProductsFile = QDir::tempPath() + "/integration_test_products_" + QString::number(QCoreApplication::applicationPid()) + ".xml";

    qDebug() << "Using temporary files:" << tempUsersFile << tempProductsFile;

    try {
        // 创建数据管理器，使用唯一的临时文件名避免冲突
        dataManager = new XMLDataManager(tempUsersFile.toStdString(), tempProductsFile.toStdString());

        // 创建加密工具和服务
        encryptionUtil = new EncryptionUtil();
        auto dataManagerShared = std::shared_ptr<XMLDataManager>(dataManager);
        authService = new AuthService(dataManagerShared, 
                                     std::shared_ptr<EncryptionUtil>(encryptionUtil));

        qDebug() << "Integration test case initialized successfully";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception during Integration initialization:" << e.what();
        QFAIL("Failed to initialize Integration test case");
    }
    catch (...) {
        qDebug() << "Unknown exception during Integration initialization";
        QFAIL("Failed to initialize Integration test case due to unknown exception");
    }
}

void TestIntegration::cleanupTestCase()
{
    qDebug() << "Cleaning up Integration test case...";

    delete authService;
    delete encryptionUtil;

    // 清理测试文件
    QString tempUsersFile = QDir::tempPath() + "/integration_test_users_" + QString::number(QCoreApplication::applicationPid()) + ".xml";
    QString tempProductsFile = QDir::tempPath() + "/integration_test_products_" + QString::number(QCoreApplication::applicationPid()) + ".xml";

    QFile::remove(tempUsersFile);
    QFile::remove(tempProductsFile);

    qDebug() << "Integration test case cleaned up";
}

void TestIntegration::testUserSaveAndRetrieve()
{
    qDebug() << "Testing User save and retrieve integration...";

    try {
        // 创建测试用户
        User testUser("1", "integration_tester", "password123", 
                      "test@example.com", "1234567890", "user");
        
        // 测试保存用户
        bool saveResult = dataManager->SaveUser(testUser);
        QVERIFY2(saveResult, "User should be saved successfully");

        // 测试获取用户
        User* retrievedUser = dataManager->GetUser("integration_tester");
        QVERIFY2(retrievedUser != nullptr, "User should be retrieved successfully");
        QCOMPARE(QString::fromStdString(retrievedUser->GetUsername()), QString("integration_tester"));
        QCOMPARE(QString::fromStdString(retrievedUser->GetEmail()), QString("test@example.com"));
        QCOMPARE(QString::fromStdString(retrievedUser->GetPhone()), QString("1234567890"));
        QCOMPARE(QString::fromStdString(retrievedUser->GetRole()), QString("user"));

        delete retrievedUser;
        qDebug() << "User save and retrieve integration test passed";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception in testUserSaveAndRetrieve:" << e.what();
        QFAIL("Exception thrown during testUserSaveAndRetrieve");
    }
    catch (...) {
        qDebug() << "Unknown exception in testUserSaveAndRetrieve";
        QFAIL("Unknown exception thrown during testUserSaveAndRetrieve");
    }
}

void TestIntegration::testUserDataIntegrity()
{
    qDebug() << "Testing User data integrity...";

    try {
        // 创建包含特殊字符的测试用户
        User testUser("2", "test_user-special", "p@ssw0rd!#$%^&*()", 
                      "test+special@example.com", "+1-234-567-8901", "admin");
        
        // 测试保存用户
        bool saveResult = dataManager->SaveUser(testUser);
        QVERIFY2(saveResult, "User with special characters should be saved successfully");

        // 测试获取用户
        User* retrievedUser = dataManager->GetUser("test_user-special");
        QVERIFY2(retrievedUser != nullptr, "User with special characters should be retrieved successfully");
        QCOMPARE(QString::fromStdString(retrievedUser->GetUsername()), QString("test_user-special"));
        QCOMPARE(QString::fromStdString(retrievedUser->GetEmail()), QString("test+special@example.com"));
        QCOMPARE(QString::fromStdString(retrievedUser->GetPhone()), QString("+1-234-567-8901"));
        QCOMPARE(QString::fromStdString(retrievedUser->GetRole()), QString("admin"));
        QVERIFY2(retrievedUser->IsAdmin(), "User should have admin role");

        delete retrievedUser;
        qDebug() << "User data integrity test passed";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception in testUserDataIntegrity:" << e.what();
        QFAIL("Exception thrown during testUserDataIntegrity");
    }
    catch (...) {
        qDebug() << "Unknown exception in testUserDataIntegrity";
        QFAIL("Unknown exception thrown during testUserDataIntegrity");
    }
}

void TestIntegration::testRegisterAndLoginFlow()
{
    qDebug() << "Testing Register and Login flow integration...";

    try {
        // 准备用户数据
        std::map<std::string, std::string> userData;
        userData["username"] = "auth_integration_tester";
        userData["password"] = "secure_password_123!";
        userData["email"] = "auth_test@example.com";
        userData["phone"] = "0987654321";
        
        // 测试注册流程
        bool registerResult = authService->Register(userData);
        QVERIFY2(registerResult, "User registration should succeed");

        // 测试登录流程
        User* loggedInUser = authService->Login("auth_integration_tester", "secure_password_123!");
        QVERIFY2(loggedInUser != nullptr, "User login should succeed");
        QCOMPARE(QString::fromStdString(loggedInUser->GetUsername()), QString("auth_integration_tester"));
        QCOMPARE(QString::fromStdString(loggedInUser->GetEmail()), QString("auth_test@example.com"));
        QCOMPARE(QString::fromStdString(loggedInUser->GetPhone()), QString("0987654321"));

        delete loggedInUser;
        qDebug() << "Register and Login flow integration test passed";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception in testRegisterAndLoginFlow:" << e.what();
        QFAIL("Exception thrown during testRegisterAndLoginFlow");
    }
    catch (...) {
        qDebug() << "Unknown exception in testRegisterAndLoginFlow";
        QFAIL("Unknown exception thrown during testRegisterAndLoginFlow");
    }
}

void TestIntegration::testPasswordEncryptionVerification()
{
    qDebug() << "Testing Password encryption and verification integration...";

    try {
        // 准备用户数据
        std::map<std::string, std::string> userData;
        userData["username"] = "encryption_tester";
        userData["password"] = "very_secure_password!@#";
        userData["email"] = "encrypt_test@example.com";
        userData["phone"] = "1122334455";
        
        // 测试注册流程（密码应该被加密）
        bool registerResult = authService->Register(userData);
        QVERIFY2(registerResult, "User registration with password encryption should succeed");

        // 测试密码验证
        bool validationResult = authService->ValidateUser("encryption_tester", "very_secure_password!@#");
        QVERIFY2(validationResult, "Password verification should succeed with correct password");

        // 测试错误密码验证
        bool wrongValidationResult = authService->ValidateUser("encryption_tester", "wrong_password");
        QVERIFY2(!wrongValidationResult, "Password verification should fail with incorrect password");

        // 测试登录
        User* loggedInUser = authService->Login("encryption_tester", "very_secure_password!@#");
        QVERIFY2(loggedInUser != nullptr, "User login with encrypted password should succeed");
        QCOMPARE(QString::fromStdString(loggedInUser->GetUsername()), QString("encryption_tester"));

        delete loggedInUser;
        qDebug() << "Password encryption and verification integration test passed";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception in testPasswordEncryptionVerification:" << e.what();
        QFAIL("Exception thrown during testPasswordEncryptionVerification");
    }
    catch (...) {
        qDebug() << "Unknown exception in testPasswordEncryptionVerification";
        QFAIL("Unknown exception thrown during testPasswordEncryptionVerification");
    }
}
