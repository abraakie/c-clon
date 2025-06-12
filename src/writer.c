//
// Created by Aki Abramowski on 04.06.25.
//

#include "writer.h"

static void write_node(Writer * writer, const Node * node);

Writer make_writer(const WriterImpl * impl, void * context) {
    return (Writer){context, impl};
}

void write_ast(Writer * writer, const Node * node) {
    if (!writer || !node) {
        return;
    }
    write_node(writer, node);
}

static void write_node(Writer * writer, const Node * node) {
    switch (node->type) {
        case NODE_OBJECT:
            writer->impl->begin_object(writer->context);
            for (const ObjectEntry * entry = node->object_entries; entry; entry = entry->next) {
                writer->impl->write_key(writer->context, entry->key);
                write_node(writer, entry->value);
            }
            writer->impl->end_object(writer->context);
            break;
        case NODE_ARRAY:
            writer->impl->begin_array(writer->context);
            for (const ArrayEntry * entry = node->array_entries; entry; entry = entry->next) {
                write_node(writer, entry->value);
            }
            writer->impl->end_array(writer->context);
            break;
        case NODE_STRING:
            writer->impl->write_string(writer->context, node->string_value);
            break;
        case NODE_NUMBER:
            writer->impl->write_number(writer->context, node->number_value);
            break;
        case NODE_BOOL:
            writer->impl->write_bool(writer->context, node->boolean_value);
            break;
        case NODE_NULL:
            writer->impl->write_null(writer->context);
            break;
    }
}
