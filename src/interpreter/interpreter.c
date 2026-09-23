//
// Created by nikita on 23.09.2026.
//

#include "interpreter.h"

#include <stddef.h>
#include <string.h>

#include "../logging/logger.h"
#include "../utils/parser.h"


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


int interpreter_eval(const char* script_path) {
    struct parser* parser = parser_create(script_path);
    if (parser == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }

    const int result = interpreter_run(parser);
    parser_destroy(parser);
    return result;
}