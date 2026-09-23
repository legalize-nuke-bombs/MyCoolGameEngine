//
// Created by nikita on 24.09.2026.
//

#include "interpreter_command_register.h"
#include "../utils/string_dictionary.h"
#include <stdlib.h>

struct interpreter_command_register {
    struct dictionary *dictionary;
};


struct interpreter_command_register* interpreter_command_register_create() {
    struct interpreter_command_register* this = malloc(sizeof(struct interpreter_command_register));
    this->dictionary = string_dictionary_build(1024);
    return this;
}
void interpreter_command_register_destroy(const struct interpreter_command_register* this) {
    dictionary_destroy(this->dictionary);
}

struct interpreter_command* interpreter_command_register_try_get_command(const struct interpreter_command_register* this, const char* command_key) {
    return dictionary_get(this->dictionary, (void*)command_key);
}