//
// Created by Aki Abramowski on 29.06.25.
//

#include <stdio.h>
#include "clon.h"

#include "json_writer.h"

void handle_error(void * context, const Error *error) {
    char buffer[512] = {0};
    format_error(error, buffer, sizeof(buffer));
    printf("%s\n", buffer);
}

int main(const int argc, const char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <clon arg> <...>\n", argv[0]);
        return 1;
    }

    char buffer[1024] = {0};
    Context context = make_pretty_indent_json_writer_context(buffer, sizeof(buffer), 2);
    if (parse_args(argv + 1, argc - 1, JSON_WRITER_IMPL, &context, handle_error, NULL)) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    printf("%s\n", buffer);
    return 0;
}
