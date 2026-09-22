//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ACTION_H
#define MYCOOLGAMEENGINE_ACTION_H

#include "list.h"

struct action_method {
    void *listener;
    void (*action)(void *listener, void *action_context);
};

struct action {
    struct list _list;
};

void action_init(struct action *this);
void action_destroy(const struct action *this);

void action_capture(struct action *this, struct action_method* action_method);
void action_invoke(const struct action *this, void* action_context);

#endif //MYCOOLGAMEENGINE_ACTION_H
