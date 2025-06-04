//
// Created by Aki Abramowski on 17.04.25.
//
#include "lexer.h"

#include <ctype.h>
#include <string.h>

static char peek(const LxrContext *ctx) {
    return ctx->input[ctx->pos];
}

static char advance(LxrContext *ctx) {
    return ctx->input[++ctx->pos];
}

LxrContext make_lxr_context(const char *input) {
    return (LxrContext) {input, 0};
}

int next_token(LxrContext *ctx, Token * token, Error * error) {
    const char * start = &ctx->input[ctx->pos];
    char c = peek(ctx);
    switch (c) {
        case '[': {
            advance(ctx);
            *token = (Token) {TOK_LBRACKET, start, 1};
            return SUCCESS;
        }
        case ']': {
            advance(ctx);
            *token = (Token) {TOK_RBRACKET, start, 1};
            return SUCCESS;
        }
        case ',': {
            advance(ctx);
            *token = (Token) {TOK_COMMA, start, 1};
            return SUCCESS;
        }
        case '=': {
            advance(ctx);
            *token = (Token) {TOK_EQUAL, start, 1};
            return SUCCESS;
        }
        case '.': {
            advance(ctx);
            *token = (Token) {TOK_DOT, start, 1};
            return SUCCESS;
        }
        case '\0': {
            *token = (Token) {TOK_EOF, start, 0};
            return SUCCESS;
        }
        default: {
            break;
        }
    }
    if (c == '-' || c == '+' || isdigit(c)) {
        do {
            c = advance(ctx);
        } while (isdigit(c) || c == '.' || c == '+' || c == '-' || c == 'e' || c == 'E');
        *token = (Token) {TOK_NUMBER, start, &ctx->input[ctx->pos] - start};
        return SUCCESS;
    }
    if (c == '"' || c == '\'') {
        const char par = c;
        char last;
        do {
            last = c;
            c = advance(ctx);
        } while (c && !(c == par && last != '\\'));
        if (c != par) {
            make_error(error, ctx->input, start, &ctx->input[ctx->pos] - start, &ctx->input[ctx->pos] - start, "Unterminated string");
            return ERROR;
        }
        advance(ctx);
        *token = (Token) {TOK_STRING, start + 1, &ctx->input[ctx->pos] - start - 2};
        return SUCCESS;
    }
    if (isalpha(c) || c == '_') {
        do {
            c = advance(ctx);
        } while (isalnum(c) || c == '_' || c == '-');
        if (&ctx->input[ctx->pos] - start <= 5) {
            if (!strncmp(start, "null", 5)) {
                *token = (Token) {TOK_NULL, start, 4};
                return SUCCESS;
            }
            if (!strncmp(start, "true", 5)) {
                *token = (Token) {TOK_TRUE, start, 4};
                return SUCCESS;
            }
            if (!strncmp(start, "false", 5)) {
                *token = (Token) {TOK_FALSE, start, 5};
                return SUCCESS;
            }
        }
        *token = (Token) {TOK_STRING, start, &ctx->input[ctx->pos] - start};
        return SUCCESS;
    }
    make_error(error, ctx->input, start, &ctx->input[ctx->pos] - start, 0, "Unknown token");
    return ERROR;
}
