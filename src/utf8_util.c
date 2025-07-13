//
// Created by Aki Abramowski on 11.07.25.
//

#include "utf8_util.h"

#include <stddef.h>

int utf8_char_len(const char c) {
    return
    (c & 0x80) == 0x00 ? 1 :
    (c & 0xE0) == 0xC0 ? 2 :
    (c & 0xF0) == 0xE0 ? 3 :
    (c & 0xF8) == 0xF0 ? 4 : 0;
}

int utf8_char_validate(const char * c) {
    if ((*c & 0x80) == 0x00) return 1;
    int l;
    unsigned int v;
    if ((*c & 0xE0) == 0xC0) {
        l = 2;
        v = *c & 0x1F;
    } else if ((*c & 0xF0) == 0xE0) {
        l = 3;
        v = *c & 0x0F;
    } else if ((*c & 0xF8) == 0xF0) {
        l = 4;
        v = 0;
    } else {
        return 0;
    }
    for (int i = 1; i < l; ++i) {
        const char cf = c[i];
        if (cf == '\0' || (cf & 0xC0) != 0x80) return 0;
        v = (v << 6) + (cf & 0x3F);
    }
    if (v > 0x10FFFF || (v >= 0xD800 && v <= 0xDFFF)) return 0;
    return l;
}

size_t utf8_str_len(const char * str) {
    size_t l = 0;
    for (; *str; ++str)
        if ((*str & 0xC0) != 0x80) ++l;
    return l;
}

size_t utf8_str_n_len(const char * str, const size_t n) {
    size_t l = 0;
    for (size_t i = 0; i < n; ++i)
        if ((str[i] & 0xC0) != 0x80) ++l;
    return l;
}
