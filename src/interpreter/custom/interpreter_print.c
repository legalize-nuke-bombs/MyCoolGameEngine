//
// Created by nikita on 24.09.2026.
//

#include "interpreter_print.h"

#include <stddef.h>
#include <stdlib.h>

#include "../interpreter_command.h"
#include "../interpreter_command_internal.h"
#include "../../logging/logger.h"
#include "../../utils/parser.h"


struct interpreter_print {
    struct interpreter_command base;
};

static const char* interpreter_print_get_key() {
    return "print";
}

static void interpreter_print_execute(const struct interpreter_command *self, struct parser *parser, const struct engine *engine) {
    const char* context = parser_next(parser);

    if (context == NULL) {
        logger_warn("Interpreter failed to find argument for `print`");
        return;
    }

    logger_info("Interpreter print: %s", context);
}

static const struct interpreter_command_vtable print_vtable = {
    .key = interpreter_print_get_key,
    .execute = interpreter_print_execute
};

struct interpreter_command* interpreter_print_create() {
    struct interpreter_print *self = malloc(sizeof(struct interpreter_print));
    self->base.vtable = &print_vtable;
    return (struct interpreter_command*)self;
}