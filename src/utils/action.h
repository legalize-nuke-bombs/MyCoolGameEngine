//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ACTION_H
#define MYCOOLGAMEENGINE_ACTION_H

struct action;

struct action* action_create(void);
void action_destroy(struct action *this);

void action_subscribe(const struct action *this, void *listener, void (*action)(void*, void*), unsigned int* subscription_token);
void action_unsubscribe(const struct action *this, unsigned int subscription_token);

void action_invoke(const struct action *this, void* action_context);

void action_clear(struct action *this);

#endif //MYCOOLGAMEENGINE_ACTION_H
