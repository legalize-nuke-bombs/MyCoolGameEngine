//
// Created by nikita on 22.09.2026.
//

#include "action.h"

#include <stdlib.h>

#include "../logging/logger.h"

struct action_method {
    void *listener;
    void (*action)(void *listener, void *action_context);
};

struct action action_create(void) {
    const struct action action = {
        ._list = list_create(1)
    };
    return action;
}
void action_destroy(struct action *this) {
    action_clear(this);
    list_destroy(&this->_list);
}

void action_subscribe(struct action *this, void *listener, void (*action)(void*, void*), unsigned int *subscription_token) {
    struct action_method *action_method = malloc(sizeof(struct action_method));
    action_method->listener = listener;
    action_method->action = action;
    *subscription_token = list_count(&this->_list);
    list_add(&this->_list, action_method);
}
void action_subscribe_no_token(struct action *this, void *listener, void (*action)(void*, void*)) {
    unsigned int recycle_bin;
    action_subscribe(this, listener, action, &recycle_bin);
}
void action_unsubscribe(const struct action *this, unsigned int subscription_token) {
    struct action_method* action_method = list_get(&this->_list, subscription_token);
    free(action_method);
    list_set(&this->_list, subscription_token, NULL);
}


void action_invoke(const struct action *this, void* action_context) {
    int ctr = 0;
    for (int i = 0; i < list_count(&this->_list); i++) {
        const struct action_method* action_method = list_get(&this->_list, i);
        if (action_method == NULL) {
            continue;
        }
        ctr++;
        action_method->action(action_method->listener, action_context);
    }
}

void action_clear(struct action *this) {
    for (int i = 0; i < list_count(&this->_list); i++) {
        struct action_method *action_method = list_get(&this->_list, i);
        if (action_method == NULL) {
            continue;
        }
        free(action_method);
    }
    list_clear(&this->_list);
}