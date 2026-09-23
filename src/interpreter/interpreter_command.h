//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H

struct parser;
struct engine;
struct interpreter_command;

const char* interpreter_command_get_key(const struct interpreter_command* this);
void interpreter_command_execute(const struct interpreter_command *this, struct parser *parser, const struct engine *engine);

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
