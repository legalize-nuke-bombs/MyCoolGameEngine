//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H

#include "../engine/engine_arguments.h"

struct parser;
struct interpreter_command;

const char* interpreter_command_get_key(const struct interpreter_command* this);
void interpreter_command_execute(const struct interpreter_command *this, struct parser *parser);

void interpreter_command_enable(struct interpreter_command *this, struct engine_arguments args);
void interpreter_command_disable(struct interpreter_command *this);

void interpreter_command_destroy(struct interpreter_command *this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
