//
// Created by Aki Abramowski on 29.06.25.
//

#include <stdio.h>
#include "clon.h"

#include "clon_json_writer.h"

int main(const int argc, const char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <clon arg> <...>\n", argv[0]);
        return 1;
    }

    char buffer[1024] = {0};
    Context context = make_pretty_json_writer_context(buffer, sizeof(buffer));
    if (parse_args(argv + 1, argc - 1, JSON_WRITER_IMPL, &context)) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    printf("%s\n", buffer);
    return 0;
}
