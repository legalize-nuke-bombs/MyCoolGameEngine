//
// Created by nikita on 23.09.2026.
//

#include "interpreter.h"

#include <stddef.h>

#include "../utils/parser.h"


int interpreter_eval(const char* script_path) {
    struct parser* parser = parser_create(script_path);
    if (parser == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }
    parser_destroy(parser);
    return INTERPRETER_OK;
}