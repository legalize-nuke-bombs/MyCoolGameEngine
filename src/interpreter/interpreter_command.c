//
// Created by nikita on 24.09.2026.
//
#include "interpreter_command.h"

#include <stdlib.h>

#include "interpreter_command_internal.h"

const char* interpreter_command_get_key(const struct interpreter_command* this) {
    return this->vtable->key(this);
}
void interpreter_command_execute(const struct interpreter_command *this, struct parser *parser) {
    this->vtable->execute(this, parser);
}

void interpreter_command_enable(struct interpreter_command *this, struct engine_arguments args) {
    if (this->vtable->on_enable) {
        this->vtable->on_enable(this, args);
    }
}
void interpreter_command_disable(struct interpreter_command *this) {
    if (this->vtable->on_disable) {
        this->vtable->on_disable(this);
    }
}

void interpreter_command_destroy(struct interpreter_command *this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}