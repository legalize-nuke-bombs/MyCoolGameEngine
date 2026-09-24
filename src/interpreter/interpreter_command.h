//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H

struct parser;
struct engine;
struct interpreter_command;

const char* interpreter_command_get_key(const struct interpreter_command* this);
void interpreter_command_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine);

void interpreter_command_destroy(struct interpreter_command *this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
