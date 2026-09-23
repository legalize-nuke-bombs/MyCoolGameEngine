//
// Created by nikita on 23.09.2026.
//

#include "interpreter.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter_command.h"
#include "interpreter_command_register.h"
#include "../logging/logger.h"
#include "../utils/parser.h"


struct interpreter {
    struct interpreter_command_register *command_register;
    const struct engine *engine;
};


struct interpreter* interpreter_create(const struct engine *engine) {
    struct interpreter* interpreter = malloc(sizeof(struct interpreter));
    interpreter->command_register = interpreter_command_register_create("Main", 128);
    interpreter->engine = engine;
    return interpreter;
}
void interpreter_destroy(struct interpreter* this) {
    interpreter_command_register_destroy(this->command_register);
    free(this);
}

static int interpreter_run(const struct interpreter *this, struct parser *parser) {
    while (1) {
        const char *word = parser_next(parser);
        if (word == NULL) {
            return INTERPRETER_OK;
        }

        const struct interpreter_command *command = interpreter_command_register_try_get_command(this->command_register, word);
        if (command == NULL) {
            continue;
        }

        interpreter_command_execute(command, parser, this->engine);
    }
}

int interpreter_eval(const struct interpreter *this, const char* script_path) {
    struct parser* parser = parser_create(script_path);
    if (parser == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }

    const int result = interpreter_run(this, parser);
    parser_destroy(parser);
    return result;
}