#include "component.h"

#include <stddef.h>

#include "../entity.h"
#include "../../logging/logger.h"

void component_init(struct component *this) {
    this->_vtable = NULL;
    this->_parent = NULL;
    this->local_position = vector2_zero;
    this->local_scale = vector2_one;
}
void component_destroy(struct component *this) {
    const char *parent_name = this->_parent != NULL ? entity_get_name(this->_parent) : "<none>";
    logger_debug("Entity %s is destroying component %s...", parent_name, component_get_key(this));
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

const char* component_get_key(const struct component *this) {
    return this->_vtable->component_key();
}

void component_update(struct component *this, const struct update_context *context) {
    if (this->_vtable->on_update != NULL) {
        this->_vtable->on_update(this, context);
    }
}
