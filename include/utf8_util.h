//
// Created by Aki Abramowski on 11.07.25.
//

#ifndef UTF8_UTIL_H
#define UTF8_UTIL_H
#include <stddef.h>

int utf8_char_len(char c);
int utf8_char_validate(const char * c);

size_t utf8_str_len(const char * str);
size_t utf8_str_n_len(const char * str, size_t n);

#endif //UTF8_UTIL_H
