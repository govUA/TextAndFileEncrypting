#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <stdexcept>
#include <chrono>

class CaesarCipher {
private:
    HMODULE hDLL;

    typedef char *(*EncryptFunc)(char *, int);

    typedef char *(*DecryptFunc)(char *, int);

    EncryptFunc encrypt;
    DecryptFunc decrypt;

public:
    CaesarCipher(const std::string &dllPath) {
        hDLL = LoadLibrary(dllPath.c_str());
        if (!hDLL) {
            throw std::runtime_error("Failed to load DLL");
        }
        encrypt = (EncryptFunc) GetProcAddress(hDLL, "encrypt");
        decrypt = (DecryptFunc) GetProcAddress(hDLL, "decrypt");
        if (!encrypt || !decrypt) {
            FreeLibrary(hDLL);
            throw std::runtime_error("Failed to get function address");
        }
    }

    ~CaesarCipher() {
        if (hDLL) {
            FreeLibrary(hDLL);
        }
    }

    std::string Encrypt(const std::string &rawText, int key) {
        char *encrypted = encrypt(const_cast<char *>(rawText.c_str()), key);
        return std::string(encrypted);
    }

    std::string Decrypt(const std::string &encryptedText, int key) {
        char *decrypted = decrypt(const_cast<char *>(encryptedText.c_str()), key);
        return std::string(decrypted);
    }
};