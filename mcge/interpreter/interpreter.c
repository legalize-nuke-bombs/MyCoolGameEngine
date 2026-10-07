//
// Created by nikita on 23.09.2026.
//

#include "interpreter.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../assets/assets.h"
#include "../logging/logger.h"
#include "../msystems/msystem.h"
#include "../scene/entity_factory.h"
#include "../scene/scene.h"
#include "../utils/action.h"
#include "../utils/dictionary.h"
#include "../utils/fields.h"
#include "../utils/string_dictionary.h"


static struct {
    struct action on_script_evaluated;

    struct dictionary *once_memory;
} interpreter;


static void interpreter_execute_list(const struct fields_list *list);

static void interpreter_eval_execute(struct fields *fields) {
    interpreter_eval(fields_get_string(fields, "path", NULL));
}

static void interpreter_print_execute(struct fields *fields) {
    logger_info("Interpreter: %s", fields_get_string(fields, "text", ""));
}

static void interpreter_once_execute(struct fields *fields) {
    const char *name = fields_get_string(fields, "name", NULL);
    const struct fields_list *body = fields_get_list(fields, "body");
    if (name == NULL) {
        logger_warn("Interpreter once (line %d) expected field `name`", fields_line(fields));
        return;
    }

    char *block_name = strdup(name);
    if (!dictionary_try_add(interpreter.once_memory, block_name, block_name)) {
        free(block_name);
        return;
    }
    interpreter_execute_list(body);
}

static void interpreter_entity_execute(struct fields *fields) {
    scene_capture_entity(entity_factory_produce(fields));
}

static const struct {
    const char *word;
    void (*execute)(struct fields *fields);
} interpreter_commands[] = {
    { "eval", interpreter_eval_execute },
    { "once", interpreter_once_execute },
    { "print", interpreter_print_execute },
    { "entity", interpreter_entity_execute },
};

static void interpreter_execute(struct fields *fields) {
    const char *word = fields_key(fields);
    for (size_t i = 0; i < sizeof(interpreter_commands) / sizeof(interpreter_commands[0]); i++) {
        if (strcmp(interpreter_commands[i].word, word) == 0) {
            interpreter_commands[i].execute(fields);
            fields_warn_unknown(fields);
            return;
        }
    }
    if (assets_add(fields)) {
        return;
    }
    logger_warn("Interpreter does not know `%s` (line %d)", word, fields_line(fields));
}

static void interpreter_execute_list(const struct fields_list *list) {
    for (int i = 0; i < fields_list_count(list); i++) {
        interpreter_execute(fields_list_get(list, i));
    }
}


static void interpreter_clear_once_memory(void) {
    struct dictionary_iterator iterator = dictionary_begin(interpreter.once_memory);
    struct dictionary_node node;
    while (dictionary_next(interpreter.once_memory, &iterator, &node)) {
        free(node.value);
    }
    dictionary_clear(interpreter.once_memory);
}

static void interpreter_on_create(void) {
    interpreter.on_script_evaluated = action_create();
    interpreter.once_memory = string_dictionary_build(3);
}
static void interpreter_on_destroy(void) {
    interpreter_clear_once_memory();
    dictionary_destroy(interpreter.once_memory);
    action_destroy(&interpreter.on_script_evaluated);
}

static void interpreter_on_disable(void) {
    interpreter_clear_once_memory();
    action_clear(&interpreter.on_script_evaluated);
}

const struct msystem interpreter_msystem = {
    .name = "interpreter",
    .on_create = interpreter_on_create,
    .on_disable = interpreter_on_disable,
    .on_destroy = interpreter_on_destroy
};

int interpreter_eval(const char* script_path) {
    if (script_path == NULL) {
        logger_warn("Interpreter expected script path, got nothing");
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }
    logger_debug("Interpreter is executing %s...", script_path);
    action_invoke(&interpreter.on_script_evaluated, (void*)script_path);

    struct fields_list *script = fields_parse_file(script_path);
    if (script == NULL) {
        return INTERPRETER_FAILED_OPEN_SCRIPT;
    }
    interpreter_execute_list(script);
    fields_list_destroy(script);

    logger_debug("Interpreter finished %s", script_path);
    return INTERPRETER_OK;
}

struct action* interpreter_get_action_on_script_evaluated(void) {
    return &interpreter.on_script_evaluated;
}
