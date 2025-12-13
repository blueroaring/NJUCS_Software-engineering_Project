#ifndef TEST_AUTHSERVICE_H
#define TEST_AUTHSERVICE_H

#include <QtTest/QTest>
#include "../services/auth_service.h"
#include "../data/xml_data_manager.h"

class TestAuthService : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // 用户注册测试
    void testRegisterValidUser();
    void testRegisterDuplicateUser();
   void testRegisterUserMissingFields();
    
    // 用户登录测试
    void testLoginValidUser();
    void testLoginInvalidUsername();
    void testLoginInvalidPassword();
    void testLoginEmptyCredentials();
    
    // 用户验证测试
    void testValidateUserValid();
    void testValidateUserInvalidUsername();
    void testValidateUserInvalidPassword();
    
    // 边界情况测试
    void testRegisterUserSpecialChars();
    void testRegisterVeryLongUserData();

private:
    AuthService* authService;
    XMLDataManager* dataManager;
};

#endif // TEST_AUTHSERVICE_H
