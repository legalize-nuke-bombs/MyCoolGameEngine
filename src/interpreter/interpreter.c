//
// Created by nikita on 23.09.2026.
//

#include "interpreter.h"

#include <stddef.h>
#include <stdlib.h>

#include "interpreter_command.h"
#include "interpreter_command_register.h"
#include "../utils/parser.h"
#include "core/interpreter_print.h"
#include "window/interpreter_window.h"
#include "../logging/logger.h"
#include "scene/interpreter_scene.h"
#include "../msystems/msystem.h"
#include "../utils/action.h"
#include "catalogs/interpreter_catalog.h"
#include "core/interpreter_eval.h"
#include "core/interpreter_ignore.h"
#include "core/interpreter_once.h"


static struct {
    struct action on_script_evaluated;

    struct interpreter_command_register *command_register;
} interpreter;


static void interpreter_on_create(void) {
    interpreter.on_script_evaluated = action_create();

    interpreter.command_register = interpreter_command_register_create("Main", 3);
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_print_as_interpreter_command(interpreter_print_create()));
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_ignore_as_interpreter_command(interpreter_ignore_create()));
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_eval_as_interpreter_command(interpreter_eval_create()));
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_once_as_interpreter_command(interpreter_once_create()));
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_catalog_create());
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_window_create());
    interpreter_command_register_capture_command(interpreter.command_register, interpreter_scene_create());
}
static void interpreter_on_destroy(void) {
    interpreter_command_register_destroy(interpreter.command_register);
    action_destroy(&interpreter.on_script_evaluated);
}

static void interpreter_on_enable(const struct engine_arguments args) {
    interpreter_command_register_enable(interpreter.command_register, args);
}
static void interpreter_on_disable(void) {
    interpreter_command_register_disable(interpreter.command_register);
    action_clear(&interpreter.on_script_evaluated);
}

const struct msystem interpreter_msystem = {
    .name = "interpreter",
    .on_create = interpreter_on_create,
    .on_enable = interpreter_on_enable,
    .on_disable = interpreter_on_disable,
    .on_destroy = interpreter_on_destroy
};

static int interpreter_parse(struct parser *parser) {
    while (1) {
        const char *word = parser_next(parser);
        if (word == NULL) {
            return INTERPRETER_OK;
        }

        const struct interpreter_command *command = interpreter_command_register_try_get_command(interpreter.command_register, word);
        if (command == NULL) {
            continue;
        }

        interpreter_command_execute(command, parser);
    }
}

static int interpreter_eval_quite(const char* script_path) {
    struct parser* parser = parser_create(script_path);
    if (parser == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }

    const int result = interpreter_parse(parser);
    parser_destroy(parser);

    return result;
}

int interpreter_eval(const char* script_path) {
    if (script_path == NULL) {
        logger_warn("Interpreter expected script path, got nothing");
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }
    logger_debug("Interpreter is executing %s...", script_path);
    action_invoke(&interpreter.on_script_evaluated, (void*)script_path);
    const int code = interpreter_eval_quite(script_path);

    if (code == INTERPRETER_OK) {
        logger_debug("Interpreter finished with exit code %d", code);
    }
    else {
        logger_debug("Interpreter finished with exit code %d", code);
    }

    return code;
}

struct action* interpreter_get_action_on_script_evaluated(void) {
    return &interpreter.on_script_evaluated;
}
