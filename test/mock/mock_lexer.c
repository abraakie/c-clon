//
// Created by Aki Abramowski on 24.05.25.
//

#include "mock_lexer.h"

#include "../helper/token_helper.h"

int next_token(LxrContext *ctx, Token * token, Error * error) {
    MockLxrContext * mock_ctx = (MockLxrContext *) ctx;
    if (mock_ctx->input_tokens_pos >= mock_ctx->input_tokens_len) {
        make_syntax_error(error, mock_ctx->input, token->start, 0, 0, "pos > input_len");
        return ERROR;
    }
    *token = mock_ctx->input_tokens[mock_ctx->input_tokens_pos++];
    mock_ctx->pos += token->length;
    return SUCCESS;
}

size_t tokens_to_string(Token * tokens, const size_t num_tokens, char * buffer, const size_t buffer_size) {
    size_t written = 0;
    for (size_t i = 0; i < num_tokens && written < buffer_size - 1; i++) {
        const size_t n = snprintf(buffer + written, buffer_size - written, "%.*s", (int)tokens[i].length, tokens[i].start);
        tokens[i].start = buffer + written;
        if (n >= buffer_size - written) {
            written = buffer_size - 1;
            break;
        }
        written += n;
    }
    buffer[written] = '\0'; \
    return written;
}
