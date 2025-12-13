#include "./test_encry.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

// 从文件读取测试数据的函数
void testWithFileData(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }
    
    // 读取整个文件内容
    std::string inputData((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    file.close();
    
    if (inputData.empty()) return;
    
    // 创建EncryptionUtil实例
    EncryptionUtil encrypt_util;
    
    // 测试哈希功能
    std::string hashed = encrypt_util.HashPassword(inputData);
    
    // 测试验证功能
    bool verified = encrypt_util.VerifyPassword(inputData, hashed);
    
    // 测试加密/解密功能
    std::string encrypted = encrypt_util.Encrypt(inputData);
    std::string decrypted = encrypt_util.Decrypt(encrypted);
}

// 从stdin读取测试数据的函数
void testWithStdinData() {
    // 读取标准输入的所有内容
    std::string inputData((std::istreambuf_iterator<char>(std::cin)),
                          std::istreambuf_iterator<char>());
    
    if (inputData.empty()) return;
    
    // 创建EncryptionUtil实例
    EncryptionUtil encrypt_util;
    
    // 测试哈希功能
    std::string hashed = encrypt_util.HashPassword(inputData);
    
    // 测试验证功能
    bool verified = encrypt_util.VerifyPassword(inputData, hashed);
    
    // 测试加密/解密功能
    std::string encrypted = encrypt_util.Encrypt(inputData);
    std::string decrypted = encrypt_util.Decrypt(encrypted);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        // 从文件读取数据进行测试
        for (int i = 1; i < argc; i++) {
            testWithFileData(argv[i]);
        }
    } else {
        // 从标准输入读取数据进行测试
        testWithStdinData();
    }
    
    return 0;
}