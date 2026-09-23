//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H

struct parser;
struct engine_context;

struct interpreter_command {
    const char *key;
    void (*func)(struct parser *parser, const struct engine_context *engine_context);
};

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
