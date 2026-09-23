//
// Created by nikita on 23.09.2026.
//

#include "interpreter.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../logging/logger.h"
#include "../utils/parser.h"


struct interpreter {
    const struct engine_context *engine_context;
};


struct interpreter* interpreter_create(const struct engine_context *engine_context) {
    struct interpreter* interpreter = malloc(sizeof(struct interpreter));
    interpreter->engine_context = engine_context;
    return interpreter;
}
void interpreter_destroy(struct interpreter* interpreter) {
    free(interpreter);
}
static int interpreter_run(struct parser *parser) {
    while (1) {
        const char *word = parser_next(parser);
        if (word == NULL) {
            return INTERPRETER_OK;
        }

        if (strcmp(word, "print") == 0) {
            const char *print_arg = parser_next(parser);
            if (print_arg != NULL) {
                logger_info("Interpreter printed %s", print_arg);
            }
        }
    }
}

int interpreter_eval(const struct interpreter *this, const char* script_path) {
    struct parser* parser = parser_create(script_path);
    if (parser == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }

    const int result = interpreter_run(parser);
    parser_destroy(parser);
    return result;
}