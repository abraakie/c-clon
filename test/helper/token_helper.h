//
// Created by Aki Abramowski on 24.05.25.
//

#ifndef HELPER_H
#define HELPER_H

#include "lexer.h"

#define MAKE_LEXER(input) ((LxrContext){input, 0})

#define TOKEN(t, s) ((Token) {t, s, sizeof(s) - 1})

#define TOKEN_NUMBER(n) TOKEN(TOK_NUMBER, n)
#define TOKEN_STRING(s) TOKEN(TOK_STRING, s)
#define TOKEN_EQUAL TOKEN(TOK_EQUAL, "=")
#define TOKEN_LBRACKET TOKEN(TOK_LBRACKET, "[")
#define TOKEN_RBRACKET TOKEN(TOK_RBRACKET, "]")
#define TOKEN_COMMA TOKEN(TOK_COMMA, ",")
#define TOKEN_DOT TOKEN(TOK_DOT, ".")
#define TOKEN_TRUE TOKEN(TOK_TRUE, "true")
#define TOKEN_FALSE TOKEN(TOK_FALSE, "false")
#define TOKEN_NULL TOKEN(TOK_NULL, "null")
#define TOKEN_EOF TOKEN(TOK_EOF, "")

const char * token_type_to_string(TokenType type);

#endif //HELPER_H
