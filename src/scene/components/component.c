#include "component_internal.h"

#include <stdlib.h>

#include "../entity.h"
#include "../../logging/logger.h"
#include "../scene.h"
#include "../../utils/rect_pair.h"

void component_base_create(struct component *this, const struct component_vtable *vtable, struct entity *parent) {
    this->vtable = vtable;
    logger_debug("Component %s is creating...", component_get_key(this));
    this->awake = false;
    this->alive = true;

    this->last_visible_chunk_update_frame_number = 0;
    this->last_simulation_chunk_update_frame_number = 0;

    this->parent = parent;
}
struct component* component_clone(const struct component *component) {
    struct component this = {0};
    this.vtable = component->vtable;
    logger_debug("Component %s is cloning...", component_get_key(component));
    this.awake = false;
    this.alive = true;
    return component->vtable->on_clone(this, component);
}

void component_awake(struct component *this) {
    if (!this->alive) {
        return;
    }
    logger_debug("Entity %s is awaking component %s...", component_get_parent_name(this), component_get_key(this));
    this->awake = true;

    if (this->vtable->on_awake) {
        this->vtable->on_awake(this);
    }
}

void component_destroy(struct component *this) {
    component_mark_destroyed(this);
    logger_debug("Entity %s is destroying component %s...", component_get_parent_name(this), component_get_key(this));
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}
void component_mark_destroyed(struct component *this) {
    if (!this->alive) {
        return;
    }
    logger_debug("Entity %s is marking destroyed component %s...", component_get_parent_name(this), component_get_key(this));
    this->alive = false;
    if (this->awake && this->vtable->on_disable) {
        this->vtable->on_disable(this);
    }
    struct scene *scene = component_get_scene(this);
    if (scene) {
        scene_notify_component_marked_destroyed(scene, this);
    }
}

bool component_is_awake(const struct component *this) {
    return this->awake;
}
bool component_is_alive(const struct component *this) {
    return this->alive;
}

void component_set_parent(struct component *this, struct entity *parent) {
    this->parent = parent;
}
struct entity* component_get_parent(const struct component *this) {
    return this->parent;
}
const char* component_get_parent_name(const struct component *this) {
    return this->parent ? entity_get_name(this->parent) : "<null>";
}
struct entity* component_get_global_parent(const struct component *this) {
    if (this->parent == NULL) return NULL;
    struct entity* iterator = this->parent;
    while (1) {
        struct entity* prev = entity_get_parent(iterator);
        if (prev == NULL) return iterator;
        iterator = prev;
    }
}
const char* component_get_global_parent_name(const struct component *this) {
    const struct entity* global_parent = component_get_global_parent(this);
    return global_parent ? entity_get_name(global_parent) : "<null>";
}
struct scene* component_get_scene(const struct component *this) {
    if (this->parent == NULL) {
        return NULL;
    }
    return entity_get_scene(this->parent);
}

struct rect component_get_rect(const struct component *this) {
    if (this->parent == NULL) {
        return rect_0;
    }
    return entity_get_rect(this->parent);
}
void component_notify_rect_changed(struct component *this, const struct rect_pair rect_pair) {
    struct component_on_rect_changed_callback_data data = {
        .component = this,
        .rect_pair = rect_pair
    };
    if (this->vtable->on_movement) {
        this->vtable->on_movement(this);
    }
    struct scene *scene = component_get_scene(this);
    if (scene) {
        scene_notify_component_resize(scene, &data);
    }
}

const char* component_get_key(const struct component *this) {
    return this->vtable->component_key();
}

void component_update(struct component *this, const struct update_context *context) {
    if (!component_is_alive(this)) {
        return;
    }
    if (this->vtable->on_update != NULL) {
        this->vtable->on_update(this, context);
    }
}
bool component_is_updateable(const struct component *this) {
    return this->vtable->on_update;
}

void component_visible_chunk_update(struct component *this, const struct update_context *context) {
    if (!component_is_alive(this)) {
        return;
    }
    if (this->vtable->on_visible_chunk_update == NULL) {
        return;
    }
    if (this->last_visible_chunk_update_frame_number == context->frame_number) {
        return;
    }
    this->last_visible_chunk_update_frame_number = context->frame_number;
    this->vtable->on_visible_chunk_update(this, context);
}
bool component_is_visible_chunkable(const struct component *this) {
    return this->vtable->on_visible_chunk_update;
}
void component_simulation_chunk_update(struct component *this, const struct update_context *context) {
    if (!component_is_alive(this)) {
        return;
    }
    if (this->vtable->on_simulation_chunk_update == NULL) {
        return;
    }
    if (this->last_simulation_chunk_update_frame_number == context->frame_number) {
        return;
    }
    this->last_simulation_chunk_update_frame_number = context->frame_number;
    this->vtable->on_simulation_chunk_update(this, context);
}
bool component_is_simulation_chunkable(const struct component* this) {
    return this->vtable->on_simulation_chunk_update;
}
bool component_is_chunkable(const struct component *this) {
    return this->vtable->on_visible_chunk_update || this->vtable->on_simulation_chunk_update || this->vtable->is_chunkable;
}