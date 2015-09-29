#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CPP_COMPILE
#include <string>
using namespace std;
#endif


char *ConstCharToChar(const char *str) {
    int len = strlen(str) + 1;
    char *buffer = (char *) malloc(sizeof(char) * len);
    strncpy(buffer, str, len);
    return buffer;
}

const char *CharToConstChar(char *str) {
    return (const char *) str;
}

#ifdef CPP_COMPILE
const char *StringToConstChar(string str) {
    return str.c_str();
}

char *StringToChar(string str) {
    char *buffer;
    int len = str.length() + 1;
    buffer = (char *) malloc(sizeof(char) * len);
    int count = str.copy(buffer, len, 0);
    buffer[count] = '\0';
    return buffer;
}
#endif

