#include "entity.h"

#include <stdlib.h>

#include "../logging/logger.h"

void entity_init(struct entity *this) {
    this->_name = "Default entity";
    list_init(&this->_components, 1);
}
void entity_destroy(const struct entity *this) {
    logger_debug("Entity %s is destroying...\n", this->_name);
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_destroy(component);
        free(component);
    }
    list_destroy(&this->_components);
}

const char* entity_get_name(const struct entity *this) {
    return this->_name;
}
void entity_set_name(struct entity *this, const char *name) {
    this->_name = name;
}

int entity_get_components_count(const struct entity *this) {
    return list_count(&this->_components);
}
void entity_capture_component(struct entity *this, struct component *component) {
    component_set_parent(component, this);
    list_add(&this->_components, component);
}

void entity_update(const struct entity *this, const struct update_context *context) {
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_update(component, context);
    }
}
