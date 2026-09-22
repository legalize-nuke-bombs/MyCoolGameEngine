#include "component.h"

#include <stddef.h>

#include "../entity.h"
#include "../../logging/logger.h"

void component_create(struct component *this) {
    this->_vtable = NULL;
    this->_parent = NULL;
    logger_debug("Entity %s is creating component...", component_get_parent_name(this));
    this->local_position = vector2_zero;
    this->local_scale = vector2_one;
}
void component_awake(struct component *this) {
    logger_debug("Entity %s is awaking component %s...", component_get_parent_name(this), component_get_key(this));
    if (this->_vtable->on_awake != NULL) {
        this->_vtable->on_awake(this);
    }
}
void component_destroy(struct component *this) {
    logger_debug("Entity %s is destroying component %s...", component_get_parent_name(this), component_get_key(this));
    if (this->_vtable->on_destroy != NULL) {
        this->_vtable->on_destroy(this);
    }
}

struct entity* component_get_parent(const struct component *this) {
    return this->_parent;
}
void component_set_parent(struct component *this, struct entity *parent) {
    this->_parent = parent;
}
const char* component_get_parent_name(const struct component *this) {
    return this->_parent != NULL ? entity_get_name(this->_parent) : "<none>";
}

const char* component_get_key(const struct component *this) {
    return this->_vtable->component_key();
}

void component_update(struct component *this, const struct update_context *context) {
    if (this->_vtable->on_update != NULL) {
        this->_vtable->on_update(this, context);
    }
}
