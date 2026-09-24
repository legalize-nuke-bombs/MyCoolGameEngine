//
// Created by nikita on 24.09.2026.
//

#include "interpreter_command_register.h"
#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"
#include "../logging/logger.h"
#include <stdlib.h>
#include <string.h>

#include "interpreter_command.h"

struct interpreter_command_register {
    const char *name;
    struct dictionary *dictionary;
};


struct interpreter_command_register* interpreter_command_register_create(const char *name, const int dim) {
    struct interpreter_command_register* this = malloc(sizeof(struct interpreter_command_register));
    this->name = name;
    this->dictionary = string_dictionary_build(dim);
    return this;
}
void interpreter_command_register_destroy(struct interpreter_command_register* this) {
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dictionary, i);
        if (node.value == NULL) {
            continue;
        }
        interpreter_command_destroy(node.value);
    }
    dictionary_destroy(this->dictionary);
    free(this);
}

void interpreter_command_register_capture_command(const struct interpreter_command_register* this, struct interpreter_command* command) {
    const char* command_key = interpreter_command_get_key(command);
    if (dictionary_try_add(this->dictionary, (void*)command_key, command)) {
        logger_debug("Interpreter command register %s registered command %s", this->name, command_key);
    }
    else {
        logger_error("Interpreter command register %s failed to register command %s", this->name, command_key);
        interpreter_command_destroy(command);
    }
}

struct interpreter_command* interpreter_command_register_try_get_command(const struct interpreter_command_register* this, const char* command_key) {
    struct interpreter_command* command = dictionary_get(this->dictionary, (void*)command_key);
    if (command == NULL) {
        char* command_list = interpreter_command_register_alloc_command_list(this);
        logger_warn("Interpreter %s does not know command %s. Supported commands: %s", this->name, command_key, command_list);
        free(command_list);
    }
    return command;
}

char* interpreter_command_register_alloc_command_list(const struct interpreter_command_register *this) {
    size_t len = 0;
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dictionary, i);
        const char* command_key = node.key;
        if (command_key == NULL) {
            continue;
        }
        len += strlen(command_key);
        len++;
    }

    char* result = malloc(len * sizeof(char) + 1);
    result[0] = '\0';
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dictionary, i);
        const char* command_key = node.key;
        if (command_key == NULL) {
            continue;
        }
        strcat(result, command_key);
        strcat(result, " ");
    }
    return result;
}