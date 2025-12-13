#include "../../includes/tests/test_encryptionutil.h"
#include <QString>
#include <QByteArray>

void TestEncryptionUtil::initTestCase()
{
    encryptUtil = new EncryptionUtil();
}

void TestEncryptionUtil::cleanupTestCase()
{
    delete encryptUtil;
}

void TestEncryptionUtil::testHashPasswordValid()
{
    QString password = "test123";
    std::string hash = encryptUtil->HashPassword(password.toStdString());
    
    QVERIFY(!hash.empty());
    QVERIFY(hash.find(':') != std::string::npos); // 应该包含盐值分隔符
    
    bool verified = encryptUtil->VerifyPassword(password.toStdString(), hash);
    QVERIFY(verified);
}

void TestEncryptionUtil::testHashPasswordDifferentSalts()
{
    QString password = "samepass";
    std::string hash1 = encryptUtil->HashPassword(password.toStdString());
    std::string hash2 = encryptUtil->HashPassword(password.toStdString());
    
    // 虽然密码相同，但由于盐值不同，哈希值应该不同
    QVERIFY(hash1 != hash2);
    
    // 但都应该能验证通过原始密码
    QVERIFY(encryptUtil->VerifyPassword(password.toStdString(), hash1));
    QVERIFY(encryptUtil->VerifyPassword(password.toStdString(), hash2));
}

void TestEncryptionUtil::testVerifyPasswordCorrect()
{
    QString password = "correct_password";
    std::string hash = encryptUtil->HashPassword(password.toStdString());
    
    bool result = encryptUtil->VerifyPassword(password.toStdString(), hash);
    QVERIFY(result);
}

void TestEncryptionUtil::testVerifyPasswordIncorrect()
{
    QString correctPassword = "correct_password";
    QString wrongPassword = "wrong_password";
    std::string hash = encryptUtil->HashPassword(correctPassword.toStdString());
    
    bool result = encryptUtil->VerifyPassword(wrongPassword.toStdString(), hash);
    QVERIFY(!result);
}

void TestEncryptionUtil::testVerifyPasswordEmpty()
{
    QString password = "";
    std::string hash = encryptUtil->HashPassword(password.toStdString());
    
    bool result = encryptUtil->VerifyPassword(password.toStdString(), hash);
    QVERIFY(result);
    
    bool resultWrong = encryptUtil->VerifyPassword("something", hash);
    QVERIFY(!resultWrong);
}

void TestEncryptionUtil::testVerifyPasswordSpecialChars()
{
    QString password = "p@ssw0rd!#$%^&*()_+-=[]{}|;':\",./<>?";
    std::string hash = encryptUtil->HashPassword(password.toStdString());
    
    bool result = encryptUtil->VerifyPassword(password.toStdString(), hash);
    QVERIFY(result);
}

void TestEncryptionUtil::testEncryptDecrypt()
{
    QString originalData = "Hello, this is test data!";
    std::string encrypted = encryptUtil->Encrypt(originalData.toStdString());
    
    QVERIFY(encrypted != originalData.toStdString());
    
    std::string decrypted = encryptUtil->Decrypt(encrypted);
    QCOMPARE(decrypted, originalData.toStdString());
}

void TestEncryptionUtil::testEncryptDecryptEmpty()
{
    QString originalData = "";
    std::string encrypted = encryptUtil->Encrypt(originalData.toStdString());
    std::string decrypted = encryptUtil->Decrypt(encrypted);
    
    QCOMPARE(decrypted, originalData.toStdString());
}

void TestEncryptionUtil::testEncryptDecryptSpecialChars()
{
    QString originalData = "!@#$%^&*()_+-={}[]|\\:;\"'<>?,./`~";
    std::string encrypted = encryptUtil->Encrypt(originalData.toStdString());
    std::string decrypted = encryptUtil->Decrypt(encrypted);
    
    QCOMPARE(decrypted, originalData.toStdString());
}

void TestEncryptionUtil::testVeryLongPassword()
{
    QString password(1000, 'a'); // 长度为1000的密码
    std::string hash = encryptUtil->HashPassword(password.toStdString());
    
    bool result = encryptUtil->VerifyPassword(password.toStdString(), hash);
    QVERIFY(result);
}

void TestEncryptionUtil::testEmptyPassword()
{
    QString password = "";
    std::string hash = encryptUtil->HashPassword(password.toStdString());
    
    // 测试空密码也能被正确验证
    bool result = encryptUtil->VerifyPassword(password.toStdString(), hash);
    QVERIFY(result);
    
    // 确保其他密码无法验证通过
    bool resultWrong = encryptUtil->VerifyPassword("wrong", hash);
    QVERIFY(!resultWrong);
}
