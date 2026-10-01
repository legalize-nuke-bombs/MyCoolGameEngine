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
#include "../subsystems/subsystem_internal.h"
#include "../utils/action.h"
#include "catalogs/interpreter_catalog.h"
#include "core/interpreter_eval.h"
#include "core/interpreter_ignore.h"
#include "core/interpreter_once.h"


struct interpreter {
    struct subsystem base;

    struct action on_script_evaluated;

    struct interpreter_command_register *command_register;
};

static const char* interpreter_get_name() {
    return "interpreter";
}
static void interpreter_on_destroy(struct subsystem* base);
void interpreter_on_enable(struct subsystem* base, struct engine_arguments args);
void interpreter_on_disable(struct subsystem* base);

static struct subsystem_vtable interpreter_vtable = {
    .name = interpreter_get_name,
    .on_destroy = interpreter_on_destroy,
    .on_enable = interpreter_on_enable,
    .on_disable = interpreter_on_disable
};


struct subsystem* interpreter_create(const struct subsystem_collection* subsystems) {
    struct interpreter* interpreter = calloc(1, sizeof(struct interpreter));
    struct subsystem* base = (struct subsystem*)interpreter;
    subsystem_create(base, &interpreter_vtable, subsystems);

    interpreter->on_script_evaluated = action_create();

    interpreter->command_register = interpreter_command_register_create("Main", 3);
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_print_as_interpreter_command(interpreter_print_create()));
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_ignore_as_interpreter_command(interpreter_ignore_create()));
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_eval_as_interpreter_command(interpreter_eval_create()));
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_once_as_interpreter_command(interpreter_once_create()));
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_catalog_create());
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_window_create());
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_scene_create());

    return base;
}
void interpreter_on_destroy(struct subsystem* base) {
    struct interpreter *this = (struct interpreter*)base;
    interpreter_command_register_destroy(this->command_register);
    action_destroy(&this->on_script_evaluated);
}

void interpreter_on_enable(struct subsystem* base, const struct engine_arguments args) {
    struct interpreter *this = (struct interpreter*)base;
    interpreter_command_register_enable(this->command_register, args);
}
void interpreter_on_disable(struct subsystem* base) {
    struct interpreter *this = (struct interpreter*)base;
    interpreter_command_register_disable(this->command_register);
    action_clear(&this->on_script_evaluated);
}

static int interpreter_parse(const struct interpreter *this, struct parser *parser) {
    while (1) {
        const char *word = parser_next(parser);
        if (word == NULL) {
            return INTERPRETER_OK;
        }

        const struct interpreter_command *command = interpreter_command_register_try_get_command(this->command_register, word);
        if (command == NULL) {
            continue;
        }

        interpreter_command_execute(command, parser, subsystem_get_subsystems((struct subsystem*)this));
    }
}

static int interpreter_eval_quite(const struct interpreter *this, const char* script_path) {
    struct parser* parser = parser_create(script_path);
    if (parser == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }

    const int result = interpreter_parse(this, parser);
    parser_destroy(parser);

    return result;
}

int interpreter_eval(const struct interpreter *this, const char* script_path) {
    if (script_path == NULL) {
        logger_warn("Interpreter expected script path, got nothing");
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }
    logger_debug("Interpreter is executing %s...", script_path);
    action_invoke(&this->on_script_evaluated, (void*)script_path);
    const int code = interpreter_eval_quite(this, script_path);

    if (code == INTERPRETER_OK) {
        logger_debug("Interpreter finished with exit code %d", code);
    }
    else {
        logger_debug("Interpreter finished with exit code %d", code);
    }

    return code;
}

struct action* interpreter_get_action_on_script_evaluated(struct interpreter *this) {
    return &this->on_script_evaluated;
}