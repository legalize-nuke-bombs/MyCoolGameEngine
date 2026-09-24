//
// Created by nikita on 24.09.2026.
//

#include "interpreter_command_parent.h"

#include <stddef.h>
#include <stdlib.h>

#include "interpreter_command_internal.h"
#include "../utils/parser.h"
#include "../logging/logger.h"
#include "interpreter_command_register.h"


struct interpreter_command_parent {
    struct interpreter_command base;
    const char* name;
    struct interpreter_command_register* command_register;
};

static const char* interpreter_command_parent_get_key(const struct interpreter_command *base) {
    return "print";
}

static void interpreter_command_parent_execute(const struct interpreter_command *base, struct parser *parser, const struct engine *engine) {
    const char* context = parser_next(parser);

    if (context == NULL) {
        logger_warn("Interpreter failed to find argument for `print`");
        return;
    }

    logger_info("Interpreter print: %s", context);
}

static void interpreter_command_parent_on_destroy(struct interpreter_command* base) {
    const struct interpreter_command_parent* this = (struct interpreter_command_parent*)base;
    interpreter_command_register_destroy(this->command_register);
}

static const struct interpreter_command_vtable command_parent_vtable = {
    .key = interpreter_command_parent_get_key,
    .execute = interpreter_command_parent_execute,
    .on_destroy = interpreter_command_parent_on_destroy
};

struct interpreter_command_parent* interpreter_command_parent_create(const char* name, int capacity) {
    struct interpreter_command_parent *this = malloc(sizeof(struct interpreter_command_parent));
    this->base.vtable = &command_parent_vtable;

    this->name = name;
    this->command_register = interpreter_command_register_create(name, capacity);

    return this;
}

void interpreter_command_parent_capture_child(const struct interpreter_command_parent* this, struct interpreter_command* child) {
    interpreter_command_register_capture_command(this->command_register, child);
}

struct interpreter_command* interpreter_command_parent_as_interpreter_command(struct interpreter_command_parent* this) {
    return (struct interpreter_command*)this;
}