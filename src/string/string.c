#include "string.h"

int strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int char_to_numeric_digit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else {
        return -1;
    }
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

int strnlen(const char *str, int max_len){
    int len = 0;
    while (str[len] != '\0' && len < max_len) {
        len++;
    }
    return len;
}

char* strcpy(char* dest, char* src){
    char* res = dest;
    while(*src!=0){
        *dest = *src;
        src+=1;
        dest+=1;
    }
    *dest = 0x00;
    return res;
}

int strncmp(const char* str1, const char* str2, int n) {
    for (int i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        if (str1[i] == '\0') {
            break;
        }
    }
    return 0;
}

int strnlen_terminator(const char* str, int max, char terminator) {
    int len = 0;
    for(int i = 0; i < max; i++) {
        if (str[i] == terminator || str[i] == '\0') {
            break;
        }
        len++;
    }
    return len;
}

int istrncmp(const char* str1, const char* str2, int n) {
    for (int i = 0; i < n; i++) {
        char c1 = str1[i];
        char c2 = str2[i];
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 32; // Convert to lowercase
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 32; // Convert to lowercase
        }
        if (c1 != c2) {
            return c1 - c2;
        }
        if (c1 == '\0') {
            break;
        }
    }
    return 0;
}