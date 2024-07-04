#include <iostream>
#include <Windows.h>

typedef char *(*EncryptFuncType)(char *, int);

typedef char *(*DecryptFuncType)(char *, int);

int main() {
    HINSTANCE hDll = LoadLibrary("CaesarLibrary.dll");
    if (hDll == NULL) {
        std::cerr << "Failed to load DLL." << std::endl;
        return 1;
    }

    EncryptFuncType encryptFunc = (EncryptFuncType) GetProcAddress(hDll, "encrypt");
    DecryptFuncType decryptFunc = (DecryptFuncType) GetProcAddress(hDll, "decrypt");
    if (encryptFunc == NULL || decryptFunc == NULL) {
        std::cerr << "Failed to find function(s) in DLL." << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    char rawText[] = "Hello World!";
    int key = 3;

    char *encryptedText = encryptFunc(rawText, key);
    std::cout << "Encrypted Text: " << encryptedText << std::endl;

    char *decryptedText = decryptFunc(encryptedText, key);
    std::cout << "Decrypted Text: " << decryptedText << std::endl;

    delete[] encryptedText;
    delete[] decryptedText;

    FreeLibrary(hDll);
    return 0;
}
