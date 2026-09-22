//
// Created by nikita on 22.09.2026.
//

#include "action.h"

#include "../logging/logger.h"


void action_init(struct action *this) {
    list_init(&this->_list, 1);
}
void action_destroy(const struct action *this) {
    list_destroy(&this->_list);
}

void action_add(struct action *this, void (*action)(void *action_context)) {
    list_add(&this->_list, action);
}
void action_invoke(const struct action *this, void* action_context) {
    logger_debug("Action invoked, actions to invoke: %d", list_count(&this->_list));
    for (int i = 0; i < list_count(&this->_list); i++) {
        void (*action)(void *action_context) = list_get(&this->_list, i);
        action(action_context);
    }
}