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

    struct action *on_component_captured;
    struct action *on_marked_destroyed;

    struct list *entities;
    struct list *components;

    struct transform *transform;

    struct entity *parent;
    struct scene *scene;
};

struct entity* entity_create(char *name, struct entity *parent, struct scene *scene) {
    struct entity *this = calloc(1, sizeof(struct entity));
    this->name = name;
    logger_debug("Entity %s is initializing...", this->name);
    this->awake = false;
    this->alive = true;

    this->on_component_captured = action_create();
    this->on_marked_destroyed = action_create();

    this->entities = list_create(1);
    this->components = list_create(1);

    this->parent = parent;
    this->scene = scene;

    return this;
}
struct entity* entity_clone(const struct entity* entity) {
    logger_debug("Entity %s is cloning...", entity->name);
    struct entity* this = calloc(1, sizeof(struct entity));
    this->name = strdup(entity->name);
    this->awake = false;
    this->alive = true;

    this->on_component_captured = action_create();
    this->on_marked_destroyed = action_create();

    this->entities = list_create(list_count(entity->entities));
    this->components = list_create(list_count(entity->components));
    for (int i = 0; i < list_count(entity->entities); i++) {
        const struct entity *child_entity = list_get(entity->entities, i);
        entity_capture_entity(this, entity_clone(child_entity));
    }
    for (int i = 0; i < list_count(entity->components); i++) {
        const struct component *component = list_get(entity->components, i);
        entity_capture_component(this, component_clone(component));
    }

    return this;
}
void entity_awake(struct entity *this) {
    if (this->awake) {
        return;
    }
    logger_debug("Entity %s is awaking...", this->name);
    this->awake = true;
    for (int i = 0; i < list_count(this->components); i++) {
        struct component *component = list_get(this->components, i);
        component_awake(component);
    }
    for (int i = 0; i < list_count(this->entities); i++) {
        struct entity *child_entity = list_get(this->entities, i);
        entity_awake(child_entity);
    }
}
void entity_destroy(struct entity *this) {
    logger_debug("Entity %s is destroying..", this->name);
    for (int i = list_count(this->entities) - 1; i >= 0; i--) {
        struct entity *child_entity = list_get(this->entities, i);
        entity_destroy(child_entity);
    }
    for (int i = list_count(this->components) - 1; i >= 0; i--) {
        struct component *component = list_get(this->components, i);
        component_destroy(component);
    }
    list_destroy(this->components);
    list_destroy(this->entities);
    action_destroy(this->on_component_captured);
    action_destroy(this->on_marked_destroyed);
    free(this->name);
    free(this);
}
void entity_mark_destroyed(struct entity *this) {
    if (!this->alive) {
        return;
    }
    logger_debug("Entity %s is marking destroyed..", this->name);
    this->alive = false;
    for (int i = list_count(this->entities) - 1; i >= 0; i--) {
        struct entity *child_entity = list_get(this->entities, i);
        entity_mark_destroyed(child_entity);
    }
    for (int i = list_count(this->components) - 1; i >= 0; i--) {
        struct component *component = list_get(this->components, i);
        component_mark_destroyed(component);
    }
    action_invoke(this->on_marked_destroyed, this);
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

void entity_set_parent(struct entity *this, struct entity *new_parent) {
    this->parent = new_parent;
}
struct entity* entity_get_parent(const struct entity *this) {
    return this->parent;
}

void entity_set_scene(struct entity *this, struct scene *new_scene) {
    this->scene = new_scene;
    for (int i = 0; i < list_count(this->entities); i++) {
        struct entity *child_entity = list_get(this->entities, i);
        entity_set_scene(child_entity, new_scene);
    }
}
struct scene* entity_get_scene(const struct entity *this) {
    return this->scene;
}

struct transform* entity_get_transform(const struct entity *this) {
    return this->transform;
}

struct action* entity_get_action_on_component_captured(const struct entity *this) {
    return this->on_component_captured;
}
struct action* entity_get_action_on_marked_destroyed(const struct entity *this) {
    return this->on_marked_destroyed;
}

void entity_capture_entity(struct entity *this, struct entity *entity) {
    logger_debug("Entity %s is capturing entity %s...", this->name, entity->name);
    entity_set_parent(entity, this);
    list_add(this->entities, entity);
    entity_recapture_components(entity);
}
void entity_capture_component(struct entity *this, struct component *component) {
    logger_debug("Entity %s is capturing component %s...", this->name, component_get_key(component));
    bool set_first = false;
    if (strcmp(component_get_key(component), "transform") == 0) {
        if (this->transform == NULL) {
            this->transform = (struct transform*)component;
            set_first = true;
        }
        else {
            logger_error("Entity %s has duplicate transform components, duplicate will not be captured and will be destroyed", this->name);
            component_destroy(component);
            component = NULL;
        }
    }
    if (component == NULL) {
        return;
    }
    component_set_parent(component, this);
    list_add(this->components, component);
    if (set_first) {
        list_swap(this->components, 0, list_count(this->components) - 1);
    }
    action_invoke(this->on_component_captured, component);
}
void entity_recapture_components(const struct entity *this) {
    logger_debug("Entity %s is recapturing all components...", this->name);
    for (int i = 0; i < list_count(this->components); i++) {
        struct component *component = list_get(this->components, i);
        action_invoke(this->on_component_captured, component);
    }
    for (int i = 0; i < list_count(this->entities); i++) {
        struct entity *child_entity = list_get(this->entities, i);
        entity_recapture_components(child_entity);
    }
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