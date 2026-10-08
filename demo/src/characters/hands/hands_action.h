//
// Created by nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HANDS_ACTION_H
#define MYCOOLGAMEENGINE_HANDS_ACTION_H
#include <stdbool.h>

struct hands_action_method {
    void *executor;
    void (*func)(void *executor);
};

struct hands_action {
    const char *name;
    bool instant;
    double duration;
    bool force;
    struct hands_action_method method;
};

#endif //MYCOOLGAMEENGINE_HANDS_ACTION_H
