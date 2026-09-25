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
    action_clear(this);
    list_destroy(this->list);
    free(this);
}

void action_subscribe(const struct action *this, void *listener, void (*action)(void*, void*), unsigned int *subscription_token) {
    struct action_method *action_method = malloc(sizeof(struct action_method));
    action_method->listener = listener;
    action_method->action = action;
    *subscription_token = list_count(this->list);
    list_add(this->list, action_method);
}
void action_unsubscribe(const struct action *this, unsigned int subscription_token) {
    struct action_method* action_method = list_get(this->list, subscription_token);
    free(action_method);
    list_set(this->list, subscription_token, NULL);
}


void action_invoke(const struct action *this, void* action_context) {
    int ctr = 0;
    for (int i = 0; i < list_count(this->list); i++) {
        const struct action_method* action_method = list_get(this->list, i);
        if (action_method == NULL) {
            continue;
        }
        ctr++;
        action_method->action(action_method->listener, action_context);
    }
}

void action_clear(struct action *this) {
    for (int i = 0; i < list_count(this->list); i++) {
        struct action_method *action_method = list_get(this->list, i);
        if (action_method == NULL) {
            continue;
        }
        free(action_method);
    }
    list_clear(this->list);
}