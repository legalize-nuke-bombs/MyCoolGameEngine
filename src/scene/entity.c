#include "entity.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "../utils/list.h"
#include "../utils/action.h"
#include "../logging/logger.h"

struct entity {
    char *name;
    bool awake;
    bool alive;
    struct list *components;
    struct action *on_component_captured;
    struct transform *transform;
    struct scene *parent;
};

struct entity* entity_create(char *name, struct scene *parent) {
    struct entity *this = malloc(sizeof(struct entity));
    this->name = name;
    logger_debug("Entity %s is initializing...", this->name);
    this->awake = false;
    this->alive = true;
    this->components = list_create(1);
    this->on_component_captured = action_create();
    this->transform = NULL;
    this->parent = parent;
    return this;
}
void entity_awake(struct entity *this) {
    logger_debug("Entity %s is awaking...", this->name);
    this->awake = true;
    this->transform = (struct transform*)entity_get_component(this, "transform");
    for (int i = 0; i < list_count(this->components); i++) {
        struct component *component = list_get(this->components, i);
        component_awake(component);
    }
}
void entity_destroy(struct entity *this) {
    logger_debug("Entity %s is destroying..", this->name);
    free(this->name);
    for (int i = 0; i < list_count(this->components); i++) {
        struct component *component = list_get(this->components, i);
        component_destroy(component);
    }
    list_destroy(this->components);
    action_destroy(this->on_component_captured);
    free(this);
}
void entity_mark_destroyed(struct entity *this) {
    if (!this->alive) {
        return;
    }
    logger_debug("Entity %s is marking destroyed..", this->name);
    this->alive = false;
    for (int i = 0; i < list_count(this->components); i++) {
        struct component *component = list_get(this->components, i);
        component_mark_destroyed(component);
    }
}

const char* entity_get_name(const struct entity *this) {
    return this->name;
}

bool entity_is_awake(const struct entity *this) {
    return this->awake;
}
bool entity_is_alive(const struct entity *this) {
    return this->alive;
}

struct scene* entity_get_parent(const struct entity *this) {
    return this->parent;
}

struct transform* entity_get_transform(const struct entity *this) {
    return this->transform;
}

void entity_subscribe_on_component_captured(struct entity *this, void *listener, void (*action)(void*, void*)) {
    action_add(this->on_component_captured, listener, action);
}

int entity_get_components_count(const struct entity *this) {
    return list_count(this->components);
}
void entity_capture_component(struct entity *this, struct component *component) {
    if (component_get_parent(component) != this) {
        logger_error("Entity %s cannot capture component %s owned by entity %s", this->name, component_get_key(component), component_get_parent_name(component));
        return;
    }
    logger_debug("Entity %s is capturing component %s", this->name, component_get_key(component));
    list_add(this->components, component);
    action_invoke(this->on_component_captured, component);
}
struct component* entity_get_component_by_index(const struct entity *this, const int index) {
    return list_get(this->components, index);
}

struct component* entity_try_get_component(const struct entity *this, const char *name) {
    for (int i = 0; i < list_count(this->components); i++) {
        struct component *component = list_get(this->components, i);
        if (strcmp(component_get_key(component), name) == 0) {
            return component;
        }
    }
    return NULL;
}
struct component* entity_get_component(const struct entity *this, const char *name) {
    struct component *component = entity_try_get_component(this, name);
    if (component == NULL) {
        logger_error("Entity %s does not contain required component %s", this->name, name);
    }
    return component;
}

void entity_update(const struct entity *this, const struct update_context *context) {
    for (int i = 0; i < list_count(this->components); i++) {
        if (!entity_is_alive(this)) {
            break;
        }
        struct component *component = list_get(this->components, i);
        component_update(component, context);
    }
}
