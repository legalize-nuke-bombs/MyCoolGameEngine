//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ACTION_H
#define MYCOOLGAMEENGINE_ACTION_H

struct action_method {
    void *_listener;
    void (*_action)(void *listener, void *action_context);
};

#define LIST_NAME action_method_list
#define LIST_TYPE struct action_method
#include "typed_list.h"

struct action {
    struct action_method_list _methods;
};

struct action action_create(void);
void action_destroy(struct action *this);

void action_subscribe(struct action *this, void *listener, void (*action)(void*, void*), unsigned int* subscription_token);
void action_subscribe_no_token(struct action *this, void *listener, void (*action)(void*, void*));
void action_unsubscribe(struct action *this, unsigned int subscription_token);

void action_invoke(const struct action *this, void* action_context);

void action_clear(struct action *this);

#endif //MYCOOLGAMEENGINE_ACTION_H
