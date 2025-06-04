//
// Created by Aki Abramowski on 24.05.25.
//

#ifndef MOCK_LEXER_H
#define MOCK_LEXER_H

#include "lexer.h"

#define MAKE_MOCK_LEXER(...) ({ \
    char input[1024]; \
    Token input_tokens[] = {__VA_ARGS__}; \
    size_t input_tokens_len = sizeof(input_tokens) / sizeof(input_tokens[0]); \
    tokens_to_string(input_tokens, input_tokens_len, input, sizeof(input)); \
    (MockLxrContext){input, 0, input_tokens, input_tokens_len, 0}; \
})

#define MAKE_MOCK_LEXER_INIT(...) \
    MockLxrContext ctx = MAKE_MOCK_LEXER(__VA_ARGS__); \
    LxrContext * ctx_ptr = (LxrContext *) &ctx; \
    Error error; \
    Error * err_ptr = &error; \
    Token token; \
    Token * tok_ptr = &token; \
    next_token(ctx_ptr, &token, &error); \

typedef struct {
    char * input;
    size_t pos;
    Token * input_tokens;
    size_t input_tokens_len;
    size_t input_tokens_pos;
} MockLxrContext;

int next_token(LxrContext *ctx, Token * token, Error * error);
size_t tokens_to_string(Token * tokens, size_t num_tokens, char * buffer, size_t buffer_size);

#endif //MOCK_LEXER_H
