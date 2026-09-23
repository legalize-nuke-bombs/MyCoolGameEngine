//
// Created by nikita on 22.09.2026.
//

#include "action.h"

#include <stdlib.h>

#include "list.h"
#include "../logging/logger.h"

struct action {
    struct list *list;
};

struct action_method {
    void *listener;
    void (*action)(void *listener, void *action_context);
};

struct action* action_create(void) {
    struct action *this = malloc(sizeof(struct action));
    this->list = list_create(1);
    return this;
}
void action_destroy(struct action *this) {
    for (int i = 0; i < list_count(this->list); i++) {
        free(list_get(this->list, i));
    }
    list_destroy(this->list);
    free(this);
}

void action_add(struct action *this, void *listener, void (*action)(void*, void*)) {
    struct action_method *action_method = malloc(sizeof(struct action_method));
    action_method->listener = listener;
    action_method->action = action;
    list_add(this->list, action_method);
}
void action_invoke(const struct action *this, void* action_context) {
    logger_debug("Action invoked, actions to invoke: %d", list_count(this->list));
    for (int i = 0; i < list_count(this->list); i++) {
        const struct action_method* action_method = list_get(this->list, i);
        action_method->action(action_method->listener, action_context);
    }
}