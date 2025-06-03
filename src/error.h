//
// Created by Aki Abramowski on 21.05.25.
//

#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>

#define SUCCESS 0
#define ERROR 1

typedef enum ErrorType {
    ERR_SYNTAX,
    ERR_MEMORY,
    ERR_OVERFLOW,
} ErrorType;

typedef struct Error {
    ErrorType type;
    const char *input;
    const char *token_start;
    size_t token_length;
    size_t pos;
    const char *message;
} Error;

void make_error(Error * error, const char * input, const char * token_start, size_t token_length, size_t local_pos, const char * message);
size_t format_error(const Error * error, char * buffer, size_t buffer_len);

#endif //COMMON_H
