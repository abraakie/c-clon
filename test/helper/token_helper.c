//
// Created by Aki Abramowski on 25.05.25.
//

#include "token_helper.h"

#include "parser.h"

const char * token_type_to_string(const TokenType type) {
    switch (type) {
        case TOK_NUMBER: return "TOK_NUMBER";
        case TOK_STRING: return "TOK_STRING";
        case TOK_EQUAL: return "TOK_EQUAL";
        case TOK_LBRACKET: return "TOK_LBRACKET";
        case TOK_RBRACKET: return "TOK_RBRACKET";
        case TOK_COMMA: return "TOK_COMMA";
        case TOK_DOT: return "TOK_DOT";
        case TOK_TRUE: return "TOK_TRUE";
        case TOK_FALSE: return "TOK_FALSE";
        case TOK_NULL: return "TOK_NULL";
        case TOK_EOF: return "TOK_EOF";
    }
    return "UNKNOWN";
}
