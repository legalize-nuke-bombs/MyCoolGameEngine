//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_INTERNAL_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_INTERNAL_H

#include "interpreter_command.h"

struct interpreter_command_vtable {
    const char* (*key)(const struct interpreter_command *base);
    void (*execute)(const struct interpreter_command *base, struct parser *parser, const struct subsystem_collection *subsystems);
    void (*on_destroy)(struct interpreter_command *base);
};

struct interpreter_command {
    const struct interpreter_command_vtable *vtable;
};

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_INTERNAL_H
