//
// Created by nikita on 24.09.2026.
//

#include "interpreter_print.h"

#include <stddef.h>
#include <stdlib.h>

#include "../interpreter_command.h"
#include "../../logging/logger.h"
#include "../../utils/parser.h"


static void interpreter_print_execute(struct parser *parser, const struct engine_context *engine_context) {
    const char* context = parser_next(parser);

    if (context == NULL) {
        logger_warn("Interpreter failed to find argument for `print`");
        return;
    }

    logger_info("Interpreter print: %s", context);
}

struct interpreter_command* interpreter_print_create() {
    struct interpreter_command *func = malloc(sizeof(struct interpreter_command));
    func->key = "print";
    func->func = interpreter_print_execute;
    return func;
}