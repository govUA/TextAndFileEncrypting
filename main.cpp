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


class Text {
private:
    std::string content;

public:
    Text() {}

    void ReadFromFile(const std::string &filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }

    void ReadFromFileInChunks(const std::string &filePath, size_t chunkSize) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        content.clear();
        char *buffer = new char[chunkSize];
        while (file.read(buffer, chunkSize) || file.gcount() > 0) {
            content.append(buffer, file.gcount());
        }

        delete[] buffer;
        file.close();
    }

    void WriteToFile(const std::string &filePath) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        file << content;
        file.close();
    }

    void WriteToFileInChunks(const std::string &filePath, size_t chunkSize) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        size_t totalSize = content.size();
        size_t written = 0;

        while (written < totalSize) {
            size_t sizeToWrite = std::min(chunkSize, totalSize - written);
            file.write(content.c_str() + written, sizeToWrite);
            written += sizeToWrite;
        }

        file.close();
    }

    std::string GetContent() const {
        return content;
    }

    void SetContent(const std::string &newContent) {
        content = newContent;
    }
};