#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char *str);

int char_to_numeric_digit(char c);

bool is_digit(char c);

int strnlen(const char *str, int max_len);

#endif