#include "library.h"

#include <stddef.h>

#include "lexer.h"
#include "parser.h"

int parse_arg(const char * arg, const WriterImpl * writer_impl, void * writer_context) {
    LxrContext ctx = {arg, 0};
    Token token;
    Error error;
    if (next_token(&ctx, &token, &error)) {
        return ERROR;
    }
    Node * node = NULL;
    if (parse_value(&ctx, &token, &node, &error)) {
        return ERROR;
    }
    Writer writer = {writer_context, writer_impl};
    write_ast(&writer, node);
    free_node(node);
    return SUCCESS;
}
