#include "entity.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "../utils/rect_pair.h"
#include "../utils/vector2_math.h"
#include "../logging/logger.h"
#include "mcge/random/random.h"



static struct rect entity_compute_rect(const struct entity *this) {
    if (this->_parent == NULL) {
        return this->_local_rect;
    }
    const struct rect parent_rect = this->_parent->_rect;
    const struct rect result = {
        .position = vector_sum(parent_rect.position, vector_multiply_vector(this->_local_rect.position, parent_rect.size)),
        .size = vector_multiply_vector(parent_rect.size, this->_local_rect.size)
    };
    return result;
}

static void entity_update_rect(struct entity *this) {
    const struct rect new_rect = entity_compute_rect(this);
    if (rects_equal(this->_rect, new_rect)) {
        return;
    }
    const struct rect_pair rect_pair = {
        .rect1 = this->_rect,
        .rect2 = new_rect
    };
    this->_rect = new_rect;
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_notify_rect_changed(component, rect_pair);
    }
    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *child_entity = list_get(&this->_entities, i);
        entity_update_rect(child_entity);
    }
}

struct entity* entity_create(char *name, struct entity *parent) {
    struct entity *this = calloc(1, sizeof(struct entity));
    this->_name = name;
    logger_debug("Entity %s is initializing...", this->_name);
    this->_id = random_next_uint128();
    this->_awake = false;
    this->_alive = true;

    this->_entities = list_create(1);
    this->_components = list_create(1);

    this->_parent = parent;

    this->_local_rect.position = vector2_zero;
    this->_local_rect.size = vector2_one;
    this->_rect = entity_compute_rect(this);

    return this;
}
void entity_awake(struct entity *this) {
    if (this->_awake) {
        return;
    }
    logger_debug("Entity %s is awaking...", this->_name);
    this->_awake = true;
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        component_awake(component);
    }
    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *child_entity = list_get(&this->_entities, i);
        entity_awake(child_entity);
    }
}
void entity_destroy(struct entity *this) {
    logger_debug("Entity %s is destroying..", this->_name);
    for (int i = list_count(&this->_entities) - 1; i >= 0; i--) {
        struct entity *child_entity = list_get(&this->_entities, i);
        entity_destroy(child_entity);
    }
    for (int i = list_count(&this->_components) - 1; i >= 0; i--) {
        struct component *component = list_get(&this->_components, i);
        component_destroy(component);
    }
    list_destroy(&this->_components);
    list_destroy(&this->_entities);
    free(this->_name);
    memset(this, (int)0xbedabedabedabeda, sizeof(struct entity)); // TODO Remove this line after the test
    free(this);
}
void entity_mark_destroyed(struct entity *this) {
    if (!this->_alive) {
        return;
    }
    logger_debug("Entity %s is marking destroyed...", this->_name);
    this->_alive = false;
    for (int i = list_count(&this->_entities) - 1; i >= 0; i--) {
        struct entity *child_entity = list_get(&this->_entities, i);
        entity_mark_destroyed(child_entity);
    }
    for (int i = list_count(&this->_components) - 1; i >= 0; i--) {
        struct component *component = list_get(&this->_components, i);
        component_mark_destroyed(component);
    }
    if (this->_in_scene) {
        scene_notify_entity_marked_destroyed(this);
    }
}

const char* entity_get_name(const struct entity *this) {
    return this->_name;
}

uint128_t entity_get_id(const struct entity *this) {
    return this->_id;
}

bool entity_is_awake(const struct entity *this) {
    return this->_awake;
}
bool entity_is_alive(const struct entity *this) {
    return this->_alive;
}

void entity_set_parent(struct entity *this, struct entity *new_parent) {
    this->_parent = new_parent;
}
struct entity* entity_get_parent(const struct entity *this) {
    return this->_parent;
}

void entity_set_in_scene(struct entity *this, const bool in_scene) {
    this->_in_scene = in_scene;
    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *child_entity = list_get(&this->_entities, i);
        entity_set_in_scene(child_entity, in_scene);
    }
}
bool entity_is_in_scene(const struct entity *this) {
    return this->_in_scene;
}

struct rect entity_get_local_rect(const struct entity *this) {
    return this->_local_rect;
}
void entity_set_local_rect(struct entity *this, const struct rect new_local_rect) {
    if (rects_equal(this->_local_rect, new_local_rect)) {
        return;
    }
    this->_local_rect = new_local_rect;
    entity_update_rect(this);
}
struct rect entity_get_rect(const struct entity *this) {
    return this->_rect;
}

void entity_capture_entity(struct entity *this, struct entity *entity) {
    logger_debug("Entity %s is capturing entity %s...", this->_name, entity->_name);
    entity_set_parent(entity, this);
    entity_update_rect(entity);
    list_add(&this->_entities, entity);
    entity_set_in_scene(entity, this->_in_scene);
    entity_recapture_components(entity);
}
void entity_capture_component(struct entity *this, struct component *component) {
    logger_debug("Entity %s is capturing component %s...", this->_name, component_get_key(component));
    component_set_parent(component, this);
    if (this->_awake) {
        component_awake(component);
    }
    list_add(&this->_components, component);
    if (this->_in_scene) {
        scene_notify_component_captured(component);
    }
}
void entity_recapture_components(const struct entity *this) {
    logger_debug("Entity %s is recapturing all components...", this->_name);
    if (!this->_in_scene) return;
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        scene_notify_component_captured(component);
    }
    for (int i = 0; i < list_count(&this->_entities); i++) {
        const struct entity *child_entity = list_get(&this->_entities, i);
        entity_recapture_components(child_entity);
    }
}

struct component* entity_try_get_component(const struct entity *this, const char *name, enum entity_query query) {
    for (int i = 0; i < list_count(&this->_components); i++) {
        struct component *component = list_get(&this->_components, i);
        if (strcmp(component_get_key(component), name) == 0) {
            return component;
        }
    }
    if (query == entity_query_recursive) {
        for (int i = 0; i < list_count(&this->_entities); i++) {
            const struct entity *child = list_get(&this->_entities, i);
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
        logger_error("Entity %s does not contain required component %s", this->_name, name);
    }
    return component;
}