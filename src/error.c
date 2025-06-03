//
// Created by Aki Abramowski on 21.05.25.
//

#include "error.h"

void make_error(Error * error, const char * input, const char * token_start, const size_t token_length, const size_t local_pos, const char * message) {
    error->type = ERR_SYNTAX;
    error->input = input;
    error->token_start = token_start;
    error->token_length = token_length;
    error->pos = token_start - input + local_pos;
    error->message = message;
}

size_t format_error(const Error *error, char *buffer, size_t buffer_len) {
    if (!error || !buffer || !buffer_len) return 0;
    size_t written = snprintf(buffer, buffer_len, "%s\n", error->input);
    if (written >= buffer_len) return buffer_len;

    const size_t token_start_pos = error->token_start - error->input;
    for (size_t i = 0; i < token_start_pos && written < buffer_len; i++) {
        buffer[written++] = ' ';
    }
    written += snprintf(buffer + written, buffer_len - written, "%.*s\n", (int) error->token_length, error->token_start);

    for (size_t i = 0; i < error->pos && written < buffer_len; i++) {
        buffer[written++] = ' ';
    }
    written += snprintf(buffer + written, buffer_len - written, "^\n%s", error->message ? error->message : "Unknown error");
    return written;
}
