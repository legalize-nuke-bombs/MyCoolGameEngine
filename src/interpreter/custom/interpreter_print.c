//
// Created by nikita on 24.09.2026.
//

#include "interpreter_print.h"

#include <stdlib.h>

#include "../interpreter_command.h"
#include "../interpreter_command_internal.h"
#include "../../logging/logger.h"
#include "../../utils/parser.h"


struct interpreter_print {
    struct interpreter_command base;
};

static const char* interpreter_scene_new_get_key(const struct interpreter_command *this) {
    return "print";
}

static void interpreter_scene_new_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine) {
    const char* context = parser_next(parser);
    logger_info("Interpreter print: %s", context);
}

static const struct interpreter_command_vtable window_new_vtable = {
    .key = interpreter_scene_new_get_key,
    .execute = interpreter_scene_new_execute,
    .on_destroy = NULL
};

struct interpreter_print* interpreter_print_create() {
    struct interpreter_print *self = malloc(sizeof(struct interpreter_print));
    self->base.vtable = &window_new_vtable;
    return self;
}

struct interpreter_command* interpreter_print_as_interpreter_command(struct interpreter_print* this) {
    return (struct interpreter_command*)this;
}