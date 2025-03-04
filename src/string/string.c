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