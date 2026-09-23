//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_FUNC_H
#define MYCOOLGAMEENGINE_INTERPRETER_FUNC_H

struct parser;
struct engine_context;

struct interpreter_func {
    const char *key;
    void (*func)(struct parser *parser, const struct engine_context *engine_context);
};

#endif //MYCOOLGAMEENGINE_INTERPRETER_FUNC_H
