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
    ERR_BUFFER_OVERFLOW,
} ErrorType;

typedef struct Error {
    ErrorType type;
    const char *input;
    const char *token_start;
    size_t token_length;
    size_t pos;
    const char *message;
} Error;

void make_syntax_error(Error * error, const char * input, const char * token_start, size_t token_length, size_t local_pos, const char * message);
void make_memory_error(Error * error, const char * message);
void make_buffer_overflow_error(Error * error, const char * message);

#endif //COMMON_H
