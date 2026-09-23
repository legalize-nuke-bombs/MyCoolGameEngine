//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_INTERNAL_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_INTERNAL_H

#include "interpreter_command.h"

struct interpreter_command_vtable {
    const char* (*key)();
    void (*execute)(const struct interpreter_command *self, struct parser *parser, const struct engine *engine);
};

struct interpreter_command {
    const struct interpreter_command_vtable *vtable;
};

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_INTERNAL_H
