//
// Created by Nikita on 26.09.2026.
//

#include "interpreter_ignore.h"

#include <stdlib.h>
#include <string.h>

#include "../interpreter_command.h"
#include "../interpreter_command_internal.h"
#include "../../utils/parser.h"


struct interpreter_ignore {
    struct interpreter_command base;
};

static const char* interpreter_ignore_get_key(const struct interpreter_command *this) {
    return "/*";
}

static void interpreter_ignore_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    const char* word;
    do {
        word = parser_next(parser);
    }
    while (word != NULL && strcmp(word, "*\\") != 0);
}

static const struct interpreter_command_vtable ignore_vtable = {
    .key = interpreter_ignore_get_key,
    .execute = interpreter_ignore_execute,
    .on_destroy = NULL
};

struct interpreter_ignore* interpreter_ignore_create() {
    struct interpreter_ignore *this = calloc(1, sizeof(struct interpreter_ignore));
    this->base.vtable = &ignore_vtable;
    return this;
}

struct interpreter_command* interpreter_ignore_as_interpreter_command(struct interpreter_ignore* this) {
    return (struct interpreter_command*)this;
}