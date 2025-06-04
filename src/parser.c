//
// Created by Aki Abramowski on 11.05.25.
//

#include "parser.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "lexer.h"

int parse_number(LxrContext * ctx, Token * token, Number * res, Error * error) {
    size_t p = 0;

    int sign = 1;
    if (token->start[p] == '-') {
        sign = -1;
        p++;
    } else if (token->start[p] == '+') {
        p++;
    }
    if (p >= token->length) {
        THROW(p, "Invalid number: Can not end here");
    }

    uint64_t int_part = 0;
    if (token->start[p] == '0') {
        p++;
        if (p < token->length && token->start[p] != '.' && token->start[p] != 'e' && token->start[p] != 'E') {
            THROW(p, "Invalid number: Expected '.', 'e', 'E'");
        }
    } else if (isdigit(token->start[p])) {
        do {
            const int digit = token->start[p] - '0';
            if (int_part > (UINT64_MAX - digit) / 10 + (sign < 0)) {
                THROW(p, "Invalid number: Number too large");
            }
            int_part = int_part * 10 + digit;
            p++;
        } while (p < token->length && isdigit(token->start[p]));
    } else {
        THROW(p, "Invalid number: Expected [0-9]");
    }

    uint64_t frac_part = 0;
    size_t frac_len = 0;
    int is_float = 0;
    if (token->start[p] == '.') {
        p++;
        if (p < token->length && !isdigit(token->start[p])) {
            THROW(p, "Invalid number: Expected [0-9]");
        }
        is_float = 1;
        do {
            const int digit = token->start[p] - '0';
            if (frac_part > (UINT64_MAX - digit) / 10 + (sign < 0)) {
                THROW(p, "Invalid number: Number too large");
            }
            frac_part = frac_part * 10 + digit;
            p++;
            frac_len++;
        } while (p < token->length && isdigit(token->start[p]));
    }

    int exp_sign = 1;
    uint64_t exp_part = 0;
    if (token->start[p] == 'e' || token->start[p] == 'E') {
        p++;
        if (token->start[p] == '-') {
            exp_sign = -1;
            p++;
        } else if (token->start[p] == '+') {
            p++;
        }
        if (!isdigit(token->start[p])) {
            THROW(p, "Invalid number: Expected [0-9]");
        }
        is_float = 1;
        exp_part = token->start[p] - '0';
        p++;
        while (p < token->length && isdigit(token->start[p])) {
            const int digit = token->start[p] - '0';
            if (int_part > (UINT64_MAX - digit) / 10 + (exp_sign < 0)) {
                THROW(p, "Invalid number: Number too large");
            }
            exp_part = exp_part * 10 + digit;
            p++;
        }
    }

    if (p != token->length) {
        make_error(error, ctx->input, token->start, token->length, ctx->input - token->start + p, "Invalid number: Unexpected symbol");
        return ERROR;
    }

    if (is_float) {
        double val = (double) int_part;
        val += (double) frac_part / pow(10, (double) frac_len);
        val *= pow(10, (double) exp_sign * (double) exp_part);
        val *= sign;
        res->type = NUMBER_FLOAT;
        res->f = val;
        TRY(next_token(ctx, token, error));
        return SUCCESS;
    }
    if (int_part > (uint64_t) INT64_MAX + (sign < 0)) {
        THROW(p, "Invalid number: Number too large");
    }
    res->type = NUMBER_INT;
    res->i = (int64_t) int_part * sign;
    TRY(next_token(ctx, token, error));
    return SUCCESS;
}

int parse_string(LxrContext * ctx, Token * token, char ** res, Error * error) {
    char * res_str = calloc(token->length, sizeof(char));
    CHECK_NOT_NULL(res_str);
    size_t i = 0;
    size_t p = 0;
    while (i < token->length) {
        if (token->start[i] == '\\') {
            i++;
            switch (token->start[i]) {
                case '\'': res_str[p] = '\''; break;
                case '"': res_str[p] = '"'; break;
                case '\\': res_str[p] = '\\'; break;
                case '/': res_str[p] = '/'; break;
                case 'b': res_str[p] = '\b'; break;
                case 'f': res_str[p] = '\f'; break;
                case 'n': res_str[p] = '\n'; break;
                case 'r': res_str[p] = '\r'; break;
                case 't': res_str[p] = '\t'; break;
                default: {
                    free(res_str);
                    THROW(p, "Invalid string: Unknown escape sequence. Expected \\, ', \", /, b, f, n, r, t");
                }
            }
        } else {
            res_str[p] = token->start[i];
        }
        i++;
        p++;
    }
    *res = res_str;
    TRY(next_token(ctx, token, error));
    return SUCCESS;
}

int parse_key_tail(LxrContext * ctx, Token * token, Node * res, ObjectEntry ** target, Error * error) {
    const int brak = token->type == TOK_LBRACKET;
    if (next_token(ctx, token, error)) {
        return ERROR;
    }
    if (token->type != TOK_STRING) {
        THROW(0, "Invalid key: Expected string");
    }
    char * key = NULL;
    TRY(parse_string(ctx, token, &key, error));
    ObjectEntry * entry = object_entry_get_entry(res->object_entries, key);
    if (!entry) {
        entry = make_object_entry(key, NULL);
        CHECK_NOT_NULL(entry);
        res->object_entries = append_object_entry(res->object_entries, entry);
    }
    if (token->type == TOK_DOT || token->type == TOK_LBRACKET) {
        if (!entry->value) {
            entry->value = make_node(NODE_OBJECT);
            CHECK_NOT_NULL(entry->value);
        }
        if (entry->value->type != NODE_OBJECT) {
            THROW(0, "Invalid object entry: Can not override existing value");
        }
        TRY(parse_key_tail(ctx, token, entry->value, &entry, error));
    } else {
        *target = entry;
    }
    if (brak) {
        if (token->type != TOK_RBRACKET) {
            THROW(0, "Invalid key: Expected ']'");
        }
        TRY(next_token(ctx, token, error));
    }
    return SUCCESS;
}

int parse_array_entry(LxrContext * ctx, Token * token, ArrayEntry ** res, Error * error) {
    Node * node;
    TRY(parse_value(ctx, token, &node, error));
    *res = append_array_entry(*res, node);
    return SUCCESS;
}

int parse_object_entry(LxrContext * ctx, Token * token, ObjectEntry ** res, Error * error) {
    if (token->type != TOK_STRING) {
        THROW(0, "Invalid object entry: Expected key");
    }
    char * key;
    TRY(parse_string(ctx, token, &key, error));
    if (token->type != TOK_LBRACKET && token->type != TOK_DOT && token->type != TOK_EQUAL) {
        THROW(0, "Invalid object entry: Expected key value pairs");
    }
    ObjectEntry * target = object_entry_get_entry(*res, key);
    if (target == NULL) {
        target = make_object_entry(key, NULL);
        CHECK_NOT_NULL(target);
        *res = append_object_entry(*res, target);
    }
    if (token->type == TOK_LBRACKET || token->type == TOK_DOT) {
        if (target->value == NULL) {
            target->value = make_node(NODE_OBJECT);
            CHECK_NOT_NULL(target->value);
        }
        TRY(parse_key_tail(ctx, token, target->value, &target, error));
    }
    if (token->type != TOK_EQUAL) {
        THROW(0, "Invalid object entry: Expected '='");
    }
    if (target->value != NULL) {
        THROW(0, "Invalid object entry: Can not override existing value");
    }
    return next_token(ctx, token, error) || parse_value(ctx, token, &target->value, error);
}

int parse_first_array_or_object_entry(LxrContext * ctx, Token * token, Node * res, Error * error) {
    if (token->type == TOK_STRING) {
        char * string;
        TRY(parse_string(ctx, token, &string, error));
        if (token->type == TOK_EQUAL || token->type == TOK_LBRACKET || token->type == TOK_DOT) {
            res->type = NODE_OBJECT;
            res->object_entries = make_object_entry(string, NULL);
            CHECK_NOT_NULL(res->object_entries);
            ObjectEntry * target = res->object_entries;
            if (token->type == TOK_LBRACKET || token->type == TOK_DOT) {
                target->value = make_node(NODE_OBJECT);
                CHECK_NOT_NULL(target->value);
                TRY(parse_key_tail(ctx, token, target->value, &target, error));
            }
            if (token->type != TOK_EQUAL) {
                THROW(0, "Invalid object entry: Expected '='");
            }
            TRY(next_token(ctx, token, error));
            TRY(parse_value(ctx, token, &target->value, error));
            return SUCCESS;
        }
        Node * value = make_node(NODE_STRING);
        CHECK_NOT_NULL(value);
        value->string_value = string;
        res->type = NODE_ARRAY;
        res->array_entries = make_array_entry(value);
        CHECK_NOT_NULL(res->array_entries);
        return SUCCESS;
    }
    Node * value = NULL;
    TRY(parse_value(ctx, token, &value, error));
    res->type = NODE_ARRAY;
    res->array_entries = make_array_entry(value);
    CHECK_NOT_NULL(res->array_entries);
    return SUCCESS;
}

int parse_array_or_object(LxrContext * ctx, Token * token, Node * res, Error * error) {
    TRY(next_token(ctx, token, error));
    if (token->type == TOK_RBRACKET) {
        res->type = NODE_ARRAY;
        return next_token(ctx, token, error);
    }
    if (token->type == TOK_EQUAL) {
        TRY(next_token(ctx, token, error));
        if (token->type != TOK_RBRACKET) {
            THROW(0, "Invalid array or object: Expected ']' for empty object");
        }
        res->type = NODE_OBJECT;
        return next_token(ctx, token, error);
    }
    TRY(parse_first_array_or_object_entry(ctx, token, res, error));
    if (res->type == NODE_ARRAY) {
        while (token->type == TOK_COMMA) {
            TRY(next_token(ctx, token, error));
            TRY(parse_array_entry(ctx, token, &res->array_entries, error));
        }
        if (token->type != TOK_RBRACKET) {
            THROW(0, "Invalid array: Expected ']' at the end");
        }
    } else {
        while (token->type == TOK_COMMA) {
            TRY(next_token(ctx, token, error));
            TRY(parse_object_entry(ctx, token, &res->object_entries, error));
        }
        if (token->type != TOK_RBRACKET) {
            THROW(0, "Invalid object: Expected ']' at the end");
        }
    }
    TRY(next_token(ctx, token, error));
    return SUCCESS;
}

int parse_value(LxrContext * ctx, Token * token, Node ** res, Error * error) {
    switch (token->type) {
        case TOK_NUMBER: {
            Number num;
            TRY(parse_number(ctx, token, &num, error));
            *res = make_node(NODE_NUMBER);
            CHECK_NOT_NULL(res);
            (*res)->number_value = num;
            break;
        }
        case TOK_STRING: {
            char * string;
            TRY(parse_string(ctx, token, &string, error));
            *res = make_node(NODE_STRING);
            CHECK_NOT_NULL(res);
            (*res)->string_value = string;
            break;
        }
        case TOK_TRUE: {
            TRY(next_token(ctx, token, error));
            *res = make_node(NODE_BOOL);
            CHECK_NOT_NULL(res);
            (*res)->boolean_value = 1;
            break;
        }
        case TOK_FALSE: {
            TRY(next_token(ctx, token, error));
            *res = make_node(NODE_BOOL);
            CHECK_NOT_NULL(res);
            (*res)->boolean_value = 0;
            break;
        }
        case TOK_NULL: {
            TRY(next_token(ctx, token, error));
            *res = make_node(NODE_NULL);
            CHECK_NOT_NULL(res);
            break;
        }
        case TOK_LBRACKET: {
            *res = make_node(NODE_ARRAY);
            CHECK_NOT_NULL(res);
            TRY(parse_array_or_object(ctx, token, *res, error));
            break;
        }
        default: {
            THROW(0, "Invalid value: Expected number, string, true, false, null, array or object");
        }
    }
    return SUCCESS;
}
