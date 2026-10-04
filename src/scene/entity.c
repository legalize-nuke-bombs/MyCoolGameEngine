#include "entity.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "../utils/list.h"
#include "../utils/rect_pair.h"
#include "../utils/vector2_math.h"
#include "../logging/logger.h"

struct entity {
    char *name;
    bool awake;
    bool alive;

    struct list entities;
    struct list components;

    struct rect local_rect;
    struct rect rect;

    struct entity *parent;
    struct scene *scene;
};

static struct rect entity_compute_rect(const struct entity *this) {
    if (this->parent == NULL) {
        return this->local_rect;
    }
    const struct rect parent_rect = this->parent->rect;
    const struct rect result = {
        .position = vector_sum(parent_rect.position, vector_multiply_vector(this->local_rect.position, parent_rect.size)),
        .size = vector_multiply_vector(parent_rect.size, this->local_rect.size)
    };
    return result;
}

static void entity_update_rect(struct entity *this) {
    const struct rect new_rect = entity_compute_rect(this);
    if (rects_equal(this->rect, new_rect)) {
        return;
    }
    const struct rect_pair rect_pair = {
        .rect1 = this->rect,
        .rect2 = new_rect
    };
    this->rect = new_rect;
    for (int i = 0; i < list_count(&this->components); i++) {
        struct component *component = list_get(&this->components, i);
        component_notify_rect_changed(component, rect_pair);
    }
    for (int i = 0; i < list_count(&this->entities); i++) {
        struct entity *child_entity = list_get(&this->entities, i);
        entity_update_rect(child_entity);
    }
}

struct entity* entity_create(char *name, struct entity *parent) {
    struct entity *this = calloc(1, sizeof(struct entity));
    this->name = name;
    logger_debug("Entity %s is initializing...", this->name);
    this->awake = false;
    this->alive = true;

    this->entities = list_create(1);
    this->components = list_create(1);

    this->parent = parent;

    this->local_rect.position = vector2_zero;
    this->local_rect.size = vector2_one;
    this->rect = entity_compute_rect(this);

    return this;
}
struct entity* entity_clone(const struct entity* entity) {
    logger_debug("Entity %s is cloning...", entity->name);
    struct entity* this = calloc(1, sizeof(struct entity));
    this->name = strdup(entity->name);
    this->awake = false;
    this->alive = true;

    this->local_rect = entity->local_rect;
    this->rect = entity->local_rect;

    this->entities = list_create(list_count(&entity->entities));
    this->components = list_create(list_count(&entity->components));
    for (int i = 0; i < list_count(&entity->entities); i++) {
        const struct entity *child_entity = list_get(&entity->entities, i);
        entity_capture_entity(this, entity_clone(child_entity));
    }
    for (int i = 0; i < list_count(&entity->components); i++) {
        const struct component *component = list_get(&entity->components, i);
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
    for (int i = 0; i < list_count(&this->components); i++) {
        struct component *component = list_get(&this->components, i);
        component_awake(component);
    }
    for (int i = 0; i < list_count(&this->entities); i++) {
        struct entity *child_entity = list_get(&this->entities, i);
        entity_awake(child_entity);
    }
}
void entity_destroy(struct entity *this) {
    logger_debug("Entity %s is destroying..", this->name);
    for (int i = list_count(&this->entities) - 1; i >= 0; i--) {
        struct entity *child_entity = list_get(&this->entities, i);
        entity_destroy(child_entity);
    }
    for (int i = list_count(&this->components) - 1; i >= 0; i--) {
        struct component *component = list_get(&this->components, i);
        component_destroy(component);
    }
    list_destroy(&this->components);
    list_destroy(&this->entities);
    free(this->name);
    free(this);
}
void entity_mark_destroyed(struct entity *this) {
    if (!this->alive) {
        return;
    }
    logger_debug("Entity %s is marking destroyed...", this->name);
    this->alive = false;
    for (int i = list_count(&this->entities) - 1; i >= 0; i--) {
        struct entity *child_entity = list_get(&this->entities, i);
        entity_mark_destroyed(child_entity);
    }
    for (int i = list_count(&this->components) - 1; i >= 0; i--) {
        struct component *component = list_get(&this->components, i);
        component_mark_destroyed(component);
    }
    if (this->scene) {
        scene_notify_entity_marked_destroyed(this->scene, this);
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

void entity_set_parent(struct entity *this, struct entity *new_parent) {
    this->parent = new_parent;
}
struct entity* entity_get_parent(const struct entity *this) {
    return this->parent;
}

void entity_set_scene(struct entity *this, struct scene *new_scene) {
    this->scene = new_scene;
    for (int i = 0; i < list_count(&this->entities); i++) {
        struct entity *child_entity = list_get(&this->entities, i);
        entity_set_scene(child_entity, new_scene);
    }
}
struct scene* entity_get_scene(const struct entity *this) {
    return this->scene;
}

struct rect entity_get_local_rect(const struct entity *this) {
    return this->local_rect;
}
void entity_set_local_rect(struct entity *this, const struct rect new_local_rect) {
    if (rects_equal(this->local_rect, new_local_rect)) {
        return;
    }
    this->local_rect = new_local_rect;
    entity_update_rect(this);
}
struct rect entity_get_rect(const struct entity *this) {
    return this->rect;
}

void entity_capture_entity(struct entity *this, struct entity *entity) {
    logger_debug("Entity %s is capturing entity %s...", this->name, entity->name);
    entity_set_parent(entity, this);
    entity_update_rect(entity);
    list_add(&this->entities, entity);
    entity_set_scene(entity, this->scene);
    entity_recapture_components(entity);
}
void entity_capture_component(struct entity *this, struct component *component) {
    logger_debug("Entity %s is capturing component %s...", this->name, component_get_key(component));
    component_set_parent(component, this);
    if (this->awake) {
        component_awake(component);
    }
    list_add(&this->components, component);
    if (this->scene) {
        scene_notify_component_captured(this->scene, component);
    }
}
void entity_recapture_components(const struct entity *this) {
    logger_debug("Entity %s is recapturing all components...", this->name);
    if (!this->scene) return;
    for (int i = 0; i < list_count(&this->components); i++) {
        struct component *component = list_get(&this->components, i);
        scene_notify_component_captured(this->scene, component);
    }
    for (int i = 0; i < list_count(&this->entities); i++) {
        const struct entity *child_entity = list_get(&this->entities, i);
        entity_recapture_components(child_entity);
    }
}

struct component* entity_try_get_component(const struct entity *this, const char *name, enum entity_query query) {
    for (int i = 0; i < list_count(&this->components); i++) {
        struct component *component = list_get(&this->components, i);
        if (strcmp(component_get_key(component), name) == 0) {
            return component;
        }
    }
    if (query == entity_query_recursive) {
        for (int i = 0; i < list_count(&this->entities); i++) {
            const struct entity *child = list_get(&this->entities, i);
            if (entity_is_alive(child)) {
                struct component* result = entity_try_get_component(child, name, query);
                if (result) {
                    return result;
                }
            }
        }
    }
    return NULL;
}
struct component* entity_get_component(const struct entity *this, const char *name, enum entity_query query) {
    struct component *component = entity_try_get_component(this, name, query);
    if (component == NULL) {
        logger_error("Entity %s does not contain required component %s", this->name, name);
    }
    return component;
}