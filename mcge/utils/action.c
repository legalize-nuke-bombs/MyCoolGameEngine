//
// Created by nikita on 22.09.2026.
//

#include "action.h"



struct action action_create(void) {
    const struct action action = {
        ._methods = action_method_list_create(1)
    };
    return action;
}
void action_destroy(struct action *this) {
    action_method_list_destroy(&this->_methods);
}

void action_subscribe(struct action *this, void *listener, void (*action)(void*, void*), unsigned int *subscription_token) {
    const struct action_method action_method = {
        ._listener = listener,
        ._action = action
    };
    *subscription_token = action_method_list_count(&this->_methods);
    action_method_list_add(&this->_methods, action_method);
}
void action_subscribe_no_token(struct action *this, void *listener, void (*action)(void*, void*)) {
    unsigned int recycle_bin;
    action_subscribe(this, listener, action, &recycle_bin);
}
void action_unsubscribe(struct action *this, unsigned int subscription_token) {
    action_method_list_get(&this->_methods, subscription_token)->_action = NULL;
}


void action_invoke(const struct action *this, void* action_context) {
    for (int i = 0; i < action_method_list_count(&this->_methods); i++) {
        const struct action_method action_method = *action_method_list_get(&this->_methods, i);
        if (action_method._action == NULL) {
            continue;
        }
        action_method._action(action_method._listener, action_context);
    }
}

void action_clear(struct action *this) {
    action_method_list_clear(&this->_methods);
}