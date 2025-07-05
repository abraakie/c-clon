//
// Created by Aki Abramowski on 21.05.25.
//

#include "error.h"

void make_syntax_error(Error * error, const char * input, const char * token_start, const size_t token_length, const size_t local_pos, const char * message) {
    error->type = ERR_SYNTAX;
    error->input = input;
    error->token_start = token_start;
    error->token_length = token_length;
    error->pos = token_start - input + local_pos;
    error->message = message;
}

void make_memory_error(Error * error, const char * message) {
    error->type = ERR_MEMORY;
    error->input = NULL;
    error->token_start = NULL;
    error->token_length = 0;
    error->pos = 0;
    error->message = message;
}

void make_buffer_overflow_error(Error * error, const char * message) {
    error->type = ERR_BUFFER_OVERFLOW;
    error->input = NULL;
    error->token_start = NULL;
    error->token_length = 0;
    error->pos = 0;
    error->message = message;
}
