//
// Created by Aki Abramowski on 17.04.25.
//
#include "lexer.h"

#include <ctype.h>
#include <string.h>

#include "utf8_util.h"

static char peek(const LxrContext *ctx) {
    return ctx->input[ctx->pos];
}

static char advance(LxrContext *ctx) {
    return ctx->input[++ctx->pos];
}

static char advance_n(LxrContext *ctx, size_t n) {
    ctx->pos += n;
    return ctx->input[ctx->pos];
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
        int last_len = 1;
        do {
            const char last = c;
            c = advance_n(ctx, last_len);
            const int len = utf8_char_validate(&ctx->input[ctx->pos]);
            if (len == 0) {
                make_syntax_error(error, ctx->input, start, &ctx->input[ctx->pos] - start, &ctx->input[ctx->pos] - start, "Invalid UTF-8 character");
                return ERROR;
            }
            if (len == 1 && c == par && !(last == '\\' && last_len == 1))
                break;
            last_len = len;
        } while (c);
        if (c != par) {
            make_syntax_error(error, ctx->input, start, &ctx->input[ctx->pos] - start, &ctx->input[ctx->pos] - start, "Unterminated string");
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
    make_syntax_error(error, ctx->input, start, &ctx->input[ctx->pos] - start, 0, "Unknown token");
    return ERROR;
}
