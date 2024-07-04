#ifndef CAESARLIBRARY_LIBRARY_H
#define CAESARLIBRARY_LIBRARY_H


#ifdef _WIN32
#ifdef CaesarLibrary_EXPORTS
#define CAESARLIBRARY_API __declspec(dllexport)
#else
#define CAESARLIBRARY_API __declspec(dllimport)
#endif
#else
#define CAESARLIBRARY_API
#endif

CAESARLIBRARY_API char *encrypt(char *rawText, int key);

CAESARLIBRARY_API char *decrypt(char *encryptedText, int key);

#endif //CAESARLIBRARY_LIBRARY_H