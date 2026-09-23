//
// Created by nikita on 24.09.2026.
//

#include "interpreter_print.h"

#include <stddef.h>

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

struct interpreter_func interpreter_print_create() {
    const struct interpreter_func func = {
        .key = "print",
        .func = interpreter_print_execute
    };
    return func;
}