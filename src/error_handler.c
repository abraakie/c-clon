//
// Created by Aki Abramowski on 04.07.25.
//

#include "error_handler.h"

size_t format_error(const Error * error, char * buffer, size_t buffer_len) {
    if (!error || !buffer || !buffer_len) return 0;

    if (error->type == ERR_BUFFER_OVERFLOW) return snprintf(buffer, buffer_len, "Buffer overflow");
    if (error->type == ERR_MEMORY) return snprintf(buffer, buffer_len, "Memory allocation failed");

    size_t written = snprintf(buffer, buffer_len, "%s\n", error->input);
    if (written >= buffer_len) return buffer_len;

    const size_t token_start_pos = error->token_start - error->input;
    for (size_t i = 0; i < token_start_pos && written < buffer_len; ++i) {
        buffer[written++] = ' ';
    }
    if (written >= buffer_len) return buffer_len;
    written += snprintf(buffer + written, buffer_len - written, "%.*s\n", (int) error->token_length, error->token_start);
    if (written >= buffer_len) return buffer_len;

    for (size_t i = 0; i < error->pos && written < buffer_len; ++i) {
        buffer[written++] = ' ';
    }
    if (written >= buffer_len) return buffer_len;
    written += snprintf(buffer + written, buffer_len - written, "^\n%s", error->message ? error->message : "Unknown error");
    return written;
}
