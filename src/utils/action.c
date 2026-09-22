//
// Created by nikita on 22.09.2026.
//

#include "action.h"

#include <stdlib.h>

#include "../logging/logger.h"


void action_init(struct action *this) {
    list_init(&this->_list, 1);
}
void action_destroy(const struct action *this) {
    for (int i = 0; i < list_count(&this->_list); i++) {
        free(list_get(&this->_list, i));
    }
    list_destroy(&this->_list);
}

void action_capture(struct action *this, struct action_method* action_method) {
    list_add(&this->_list, action_method);
}
void action_invoke(const struct action *this, void* action_context) {
    logger_debug("Action invoked, actions to invoke: %d", list_count(&this->_list));
    for (int i = 0; i < list_count(&this->_list); i++) {
        const struct action_method* action_method = list_get(&this->_list, i);
        action_method->action(action_method->listener, action_context);
    }
}