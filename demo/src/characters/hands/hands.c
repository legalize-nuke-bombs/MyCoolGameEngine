//
// Created by nikita on 08.10.2026.
//

#include "hands.h"

#include <mcge/mcge.h>


struct hands {
    struct component component;
    struct hands_action action;

    struct action on_changed;
};

const char* hands_component_key(void) {
    return "hands";
}

static void hands_on_create(struct component* base, struct fields *fields) {
    struct hands *this = (struct hands*)base;
    this->on_changed = action_create();
}
static void hands_on_destroy(struct component* base) {
    struct hands *this = (struct hands*)base;
    action_destroy(&this->on_changed);
}

static void hands_execute_active_action(struct hands *this);

static void hands_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct hands *this = (struct hands*)base;
    if (this->action.duration == 0) {
        return;
    }
    this->action.duration = this->action.duration - context->dt;
    if (this->action.duration <= 0) {
        hands_execute_active_action(this);
    }
    action_invoke(&this->on_changed, NULL);
}

const struct component_vtable hands_vtable = {
    .component_key = hands_component_key,
    .size = sizeof(struct hands),
    .on_create = hands_on_create,
    .on_destroy = hands_on_destroy,
    .on_simulation_chunk_update = hands_simulation_chunk_update
};

bool hands_try_put(struct hands* this, const struct hands_action action) {
    if (this->action.duration > 0 && this->action.priority >= action.priority) {
        logger_debug("Entity %s failed to put in hands queue action %s because action %s with higher priority is active", component_get_global_parent_name((struct component*)this), action.name, this->action.name);
        return false;
    }
    logger_debug("Entity %s put in hands queue action %s", component_get_global_parent_name((struct component*)this), action.name);
    this->action = action;
    if (this->action.duration == 0) {
        hands_execute_active_action(this);
    }
    action_invoke(&this->on_changed, NULL);
    return true;
}

static void hands_execute_active_action(struct hands* this) {
    logger_debug("Entity %s executed hands action %s", component_get_global_parent_name((struct component*)this), this->action.name);
    this->action.duration = 0;
    this->action.method.func(this->action.method.executor, this->action.method.context);
}