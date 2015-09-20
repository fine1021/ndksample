#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#ifdef __cplusplus
}
#endif