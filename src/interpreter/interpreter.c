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
#include "renderer_layer_manager/interpreter_renderer_layer_manager.h"
#include "window/interpreter_window.h"
#include "../logging/logger.h"
#include "scene/interpreter_scene.h"
#include "../subsystems/subsystem_internal.h"


struct interpreter {
    struct subsystem base;

    struct interpreter_command_register *command_register;
};

static const char* interpreter_get_name() {
    return "interpreter";
}
static void interpreter_on_destroy(struct subsystem* base);

static struct subsystem_vtable interpreter_vtable = {
    .name = interpreter_get_name,
    .on_destroy = interpreter_on_destroy,
    .on_enable = NULL,
    .on_disable = NULL
};


struct subsystem* interpreter_create(const struct subsystem_collection* subsystems) {
    struct interpreter* interpreter = malloc(sizeof(struct interpreter));
    struct subsystem* base = (struct subsystem*)interpreter;
    subsystem_create(base, &interpreter_vtable, subsystems);

    interpreter->command_register = interpreter_command_register_create("Main", 5);
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_print_as_interpreter_command(interpreter_print_create()));
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_renderer_layer_manager_create());
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_window_create());
    interpreter_command_register_capture_command(interpreter->command_register, interpreter_scene_create());

    return base;
}
void interpreter_on_destroy(struct subsystem* base) {
    const struct interpreter *this = (struct interpreter*)base;
    interpreter_command_register_destroy(this->command_register);
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
    logger_info("Interpreter is executing %s...", script_path);

    const int code = interpreter_eval_quite(this, script_path);

    if (code == INTERPRETER_OK) {
        logger_info("Interpreter finished with exit code %d", code);
    }
    else {
        logger_error("Interpreter finished with exit code %d", code);
    }

    return code;
}