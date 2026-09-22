//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ACTION_H
#define MYCOOLGAMEENGINE_ACTION_H

#include "list.h"

struct action {
    struct list _list;
};

void action_init(struct action *this);
void action_destroy(const struct action *this);

void action_add(struct action *this, void *listener, void (*action)(void*, void*));
void action_invoke(const struct action *this, void* action_context);

#endif //MYCOOLGAMEENGINE_ACTION_H
