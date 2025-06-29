//
// Created by Aki Abramowski on 04.06.25.
//

#ifndef WRITER_H
#define WRITER_H
#include "ast.h"

typedef void (*WriterImplementation) (void * context, const Node * node);

#endif //WRITER_H
