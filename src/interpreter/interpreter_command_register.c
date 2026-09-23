//
// Created by nikita on 24.09.2026.
//

#include "interpreter_command_register.h"
#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"
#include "../logging/logger.h"
#include <stdlib.h>

#include "interpreter_command.h"

#include "custom/interpreter_print.h"

struct interpreter_command_register {
    struct dictionary *dictionary;
};


static void interpreter_command_register_capture_command(const struct interpreter_command_register* this, struct interpreter_command* command) {
    logger_debug("Interpreter is registering command %s...", command->key);
    if (!dictionary_try_add(this->dictionary, (void*)command->key, command)) {
        logger_error("Interpreter failed to register command %s", command->key);
    }
}

static void interpreter_command_register_register_all(const struct interpreter_command_register* this) {
    interpreter_command_register_capture_command(this, interpreter_print_create());
}


struct interpreter_command_register* interpreter_command_register_create() {
    struct interpreter_command_register* this = malloc(sizeof(struct interpreter_command_register));
    this->dictionary = string_dictionary_build(1024);
    interpreter_command_register_register_all(this);
    return this;
}
void interpreter_command_register_destroy(const struct interpreter_command_register* this) {
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dictionary, i);
        if (node.value == NULL) {
            continue;
        }
        free(node.value);
    }
    dictionary_destroy(this->dictionary);
}

struct interpreter_command* interpreter_command_register_try_get_command(const struct interpreter_command_register* this, const char* command_key) {
    return dictionary_get(this->dictionary, (void*)command_key);
}