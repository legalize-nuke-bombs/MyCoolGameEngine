//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ACTION_H
#define MYCOOLGAMEENGINE_ACTION_H

struct action;

struct action* action_create(void);
void action_destroy(struct action *this);

void action_add(struct action *this, void *listener, void (*action)(void*, void*));
void action_invoke(const struct action *this, void* action_context);

#endif //MYCOOLGAMEENGINE_ACTION_H
