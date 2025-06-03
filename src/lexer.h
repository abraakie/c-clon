//
// Created by Aki Abramowski on 17.04.25.
//

#ifndef LEXER_H
#define LEXER_H

#include "error.h"

typedef enum {
    TOK_NUMBER,
    TOK_STRING,
    TOK_EQUAL,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_COMMA,
    TOK_DOT,
    TOK_TRUE,
    TOK_FALSE,
    TOK_NULL,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    size_t length;
} Token;

typedef struct {
    const char * input;
    size_t pos;
} LxrContext;

int next_token(LxrContext *ctx, Token * token, Error * error);

#endif //LEXER_H
