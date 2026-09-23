//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H

struct parser;
struct engine;

struct interpreter_command {
    const char *key;
    void (*func)(struct parser *parser, const struct engine *engine);
};

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_H
