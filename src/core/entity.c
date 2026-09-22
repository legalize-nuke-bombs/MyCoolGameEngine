#include "entity.h"

#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "../logging/logger.h"

void entity_create(struct entity *this) {
    this->_name = "Default entity";
    logger_debug("Entity %s is initializing...", this->_name);
    list_init(&this->_components, 1);
    action_init(&this->on_component_captured);
    this->transform = NULL;
}
void entity_awake(struct entity *this) {
    logger_debug("Entity %s is awaking...", this->_name);
    this->transform = (struct transform*)entity_get_component(this, "transform");
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_awake(component);
    }
}
void entity_destroy(const struct entity *this) {
    logger_debug("Entity %s is destroying...", this->_name);
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_destroy(component);
        free(component);
    }
    list_destroy(&this->_components);
    action_destroy(&this->on_component_captured);
}

const char* entity_get_name(const struct entity *this) {
    return this->_name;
}
void entity_set_name(struct entity *this, const char *name) {
    logger_debug("Entity %s is renaming to %s", this->_name, name);
    this->_name = name;
}

int entity_get_components_count(const struct entity *this) {
    return list_count(&this->_components);
}
void entity_capture_component(struct entity *this, struct component *component) {
    logger_debug("Entity %s is capturing component %s", this->_name, component_get_key(component));
    component_set_parent(component, this);
    list_add(&this->_components, component);
    action_invoke(&this->on_component_captured, component);
}
struct component* entity_get_component_by_index(const struct entity *this, const int index) {
    return list_get(&this->_components, index);
}

struct component* entity_try_get_component(const struct entity *this, const char *name) {
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        if (strcmp(component_get_key(component), name) == 0) {
            return component;
        }
    }
    return NULL;
}
struct component* entity_get_component(const struct entity *this, const char *name) {
    struct component *component = entity_try_get_component(this, name);
    if (component == NULL) {
        logger_error("Entity %s does not contain required component %s", this->_name, name);
    }
    return component;
}

void entity_update(const struct entity *this, const struct update_context *context) {
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_update(component, context);
    }
}
