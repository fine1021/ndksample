#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifdef __cplusplus
extern "C" {
#endif

char *constCharToChar(const char *str) {
    int len = strlen(str) + 1;
    char *buffer = (char *) malloc(sizeof(char) * len);
    strncpy(buffer, str, len);
    return buffer;
}

const char *charToConstChar(char *str) {
    return (const char *) str;
}

//const char *stringToConstChar(string str) {
//    return str.c_str();
//}

#ifdef __cplusplus
}
#endif
#pragma clang diagnostic pop