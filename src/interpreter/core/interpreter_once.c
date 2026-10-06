//
// Created by Nikita on 01.10.2026.
//

#include "interpreter_once.h"

#include <stdlib.h>
#include <string.h>

#include "../interpreter_command_internal.h"
#include "../../logging/logger.h"
#include "../../utils/dictionary.h"
#include "../../utils/parser.h"
#include "../../utils/string_dictionary.h"

struct interpreter_once {
    struct interpreter_command base;
    struct dictionary* memory;
};

static const char* interpreter_once_get_key(const struct interpreter_command *this) {
    return "once";
}

static void interpreter_once_execute(const struct interpreter_command *base, struct parser *parser) {
    const struct interpreter_once *this = (const struct interpreter_once*)base;

    char* block_name = parser_next_dup(parser);
    if (block_name == NULL) {
        return;
    }

    const char* command = parser_next(parser);
    if (command == NULL) {
        free(block_name);
        return;
    }
    if (strcmp(command, "start") == 0) {}
    else if (strcmp(command, "end") == 0) {
        free(block_name);
        return;
    }
    else {
        logger_warn("Interpreter once block %s unknown argument %s. Supported: start end", block_name, command);
        free(block_name);
        return;
    }

    if (dictionary_try_add(this->memory, block_name, block_name)) {
        return;
    }

    while (1) {
        const char *token = parser_next(parser);
        if (token == NULL) {
            break;
        }
        if (strcmp(token, interpreter_once_get_key(base)) == 0) {
            token = parser_next(parser);
            if (token == NULL) {
                break;
            }
            if (strcmp(token, block_name) == 0) {
                token = parser_next(parser);
                if (token == NULL) {
                    break;
                }
                if (strcmp(token, "end") == 0) {
                    break;
                }
            }
        }
    }

    free(block_name);
}

static void interpreter_once_clear_memory(struct interpreter_once *this) {
    struct dictionary_iterator memory_iterator = dictionary_begin(this->memory);
    struct dictionary_node node;
    while (dictionary_next(this->memory, &memory_iterator, &node)) {
        free(node.value);
    }
    dictionary_clear(this->memory);
}

static void interpreter_once_on_destroy(struct interpreter_command *base);
static void interpreter_once_on_enable(struct interpreter_command *base, struct engine_arguments args);
static void interpreter_once_on_disable(struct interpreter_command *base);

static const struct interpreter_command_vtable once_vtable = {
    .key = interpreter_once_get_key,
    .execute = interpreter_once_execute,
    .on_destroy = interpreter_once_on_destroy,
    .on_enable = interpreter_once_on_enable,
    .on_disable = interpreter_once_on_disable
};

struct interpreter_once* interpreter_once_create() {
    struct interpreter_once *this = calloc(1, sizeof(struct interpreter_once));
    this->base.vtable = &once_vtable;
    this->memory = string_dictionary_build(3);
    return this;
}
static void interpreter_once_on_destroy(struct interpreter_command *base) {
    struct interpreter_once *this = (struct interpreter_once*)base;
    interpreter_once_clear_memory(this);
    dictionary_destroy(this->memory);
}

static void interpreter_once_on_enable(struct interpreter_command *base, struct engine_arguments args) {
    struct interpreter_once *this = (struct interpreter_once*)base;
}
static void interpreter_once_on_disable(struct interpreter_command *base) {
    struct interpreter_once *this = (struct interpreter_once*)base;
    interpreter_once_clear_memory(this);
}

struct interpreter_command* interpreter_once_as_interpreter_command(struct interpreter_once* this) {
    return (struct interpreter_command*)this;
}