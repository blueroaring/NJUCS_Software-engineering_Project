#ifndef TEST_ENCRYPTIONUTIL_H
#define TEST_ENCRYPTIONUTIL_H

#include <QtTest/QTest>
#include "../services/encryption_util.h"

class TestEncryptionUtil : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 测试密码哈希功能
    void testHashPasswordValid();
    void testHashPasswordDifferentSalts();
    
    // 测试密码验证功能
    void testVerifyPasswordCorrect();
    void testVerifyPasswordIncorrect();
    void testVerifyPasswordEmpty();
    void testVerifyPasswordSpecialChars();
    
    // 测试加密解密功能
    void testEncryptDecrypt();
    void testEncryptDecryptEmpty();
    void testEncryptDecryptSpecialChars();
    
    // 边界测试
    void testVeryLongPassword();
    void testEmptyPassword();

private:
    EncryptionUtil* encryptUtil;
};

#endif // TEST_ENCRYPTIONUTIL_H
