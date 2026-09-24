//
// Created by nikita on 24.09.2026.
//
#include "interpreter_command.h"

#include <stdlib.h>

#include "interpreter_command_internal.h"

const char* interpreter_command_get_key(const struct interpreter_command* this) {
    return this->vtable->key(this);
}
void interpreter_command_execute(const struct interpreter_command *this, struct parser *parser, const struct engine *engine) {
    this->vtable->execute(this, parser, engine);
}

void interpreter_command_destroy(struct interpreter_command *this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}