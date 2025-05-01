#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char *str);

int char_to_numeric_digit(char c);

bool is_digit(char c);

int strnlen(const char *str, int max_len);

char* strcpy(char* dest, char* src);

int strncmp(const char* str1, const char* str2, int n);

int strnlen_terminator(const char* str, int max, char terminator);

int istrncmp(const char* str1, const char* str2, int n);

#endif