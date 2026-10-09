//
// Created by nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HANDS_ACTION_H
#define MYCOOLGAMEENGINE_HANDS_ACTION_H
#include <stdbool.h>

struct hands_action_method {
    void *executor;
    void *context;
    void (*func)(void *executor, void *context);
};

struct hands_action {
    const char *name;
    double duration;
    unsigned char priority;
    struct hands_action_method method;
};

#endif //MYCOOLGAMEENGINE_HANDS_ACTION_H
