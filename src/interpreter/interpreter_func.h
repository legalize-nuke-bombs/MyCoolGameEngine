//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_FUNC_H
#define MYCOOLGAMEENGINE_INTERPRETER_FUNC_H

struct parser;

struct interpreter_func {
    const char *key;
    void (*action)(struct parser *parser);
};

#endif //MYCOOLGAMEENGINE_INTERPRETER_FUNC_H
