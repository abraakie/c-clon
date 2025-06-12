//
// Created by Aki Abramowski on 11.05.25.
//

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

#define TRY(expr) if ((expr) != SUCCESS) { return ERROR; }
#define THROW(pos, msg) do { make_error(error, ctx->input, token->start, token->length, pos, msg); return ERROR; } while (0)
#define CHECK_NOT_NULL(ptr) if (!(ptr)) { make_error(error, ctx->input, token->start, token->length, 0, "Memory allocation failed"); return ERROR; }

int parse_first_array_or_object_entry(LxrContext * ctx, Token * token, Node * res, Error * error);
int parse_array_entry(LxrContext * ctx, Token * token, ArrayEntry ** res, Error * error);
int parse_object_entry(LxrContext * ctx, Token * token, ObjectEntry ** res, Error * error);
int parse_value(LxrContext * ctx, Token * token, Node ** res, Error * error);
int parse_array_or_object(LxrContext * ctx, Token * token, Node * res, Error * error);
int parse_number(LxrContext * ctx, Token * token, Number * res, Error * error);
int parse_string(LxrContext * ctx, Token * token, char ** res, Error * error);

#endif //PARSER_H
