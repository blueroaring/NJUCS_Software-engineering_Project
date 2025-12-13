#ifndef TEST_INTEGRATION_H
#define TEST_INTEGRATION_H

#include <QtTest/QTest>
#include "../services/auth_service.h"
#include "../services/encryption_util.h"
#include "../data/xml_data_manager.h"
#include "../models/user.h"

class TestIntegration : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // XMLDataManager 与 User 模型集成测试
    void testUserSaveAndRetrieve();
    void testUserDataIntegrity();
    
    // AuthService 与 EncryptionUtil 集成测试
    void testRegisterAndLoginFlow();
    void testPasswordEncryptionVerification();

private:
    XMLDataManager* dataManager;
    AuthService* authService;
    EncryptionUtil* encryptionUtil;
};

#endif // TEST_INTEGRATION_H