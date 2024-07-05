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


class CommandLineInterface {
public:
    void Run(bool chunks) {
        if (chunks) {
            std::cout << "Chunk-based logic on:" << std::endl;
        } else {
            std::cout << "Chunk-based logic off:" << std::endl;
        }
        int chunkSize = 1024;
        std::string operation, inputFilePath, outputFilePath;
        int key;
        std::cout << "Choose operation (encrypt/decrypt): ";
        std::cin >> operation;
        std::cout << "Enter input file path: ";
        std::cin >> inputFilePath;
        std::cout << "Enter output file path: ";
        std::cin >> outputFilePath;
        std::cout << "Enter key: ";
        std::cin >> key;

        CaesarCipher cipher("CaesarLibrary.dll");
        Text text;

        auto start = std::chrono::high_resolution_clock::now();
        try {
            if (chunks) {
                text.ReadFromFileInChunks(inputFilePath, chunkSize);
            } else {
                text.ReadFromFile(inputFilePath);
            }
            std::string content = text.GetContent();
            std::string result;

            if (operation == "encrypt") {
                result = cipher.Encrypt(content, key);
            } else if (operation == "decrypt") {
                result = cipher.Decrypt(content, key);
            } else {
                throw std::runtime_error("Invalid operation");
            }

            text.SetContent(result);
            if (chunks) {
                text.WriteToFileInChunks(outputFilePath, 1024);
            } else {
                text.WriteToFile(outputFilePath);
            }
            std::cout << "Operation completed successfully." << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << duration.count() << std::endl;
    }
};