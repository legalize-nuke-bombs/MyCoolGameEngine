#include "component_internal.h"

#include <stdlib.h>

#include "../entity.h"
#include "../../logging/logger.h"
#include "../../utils/vector2_math.h"
#include "custom/transform.h"

void component_init(struct component *this, const struct component_vtable *vtable) {
    this->vtable = vtable;
    this->parent = NULL;
    logger_debug("Entity %s is creating component...", component_get_parent_name(this));
    this->local_position = vector2_zero;
    this->local_scale = vector2_one;
}
void component_awake(struct component *this) {
    logger_debug("Entity %s is awaking component %s...", component_get_parent_name(this), component_get_key(this));
    if (this->vtable->on_awake != NULL) {
        this->vtable->on_awake(this);
    }
}
void component_destroy(struct component *this) {
    logger_debug("Entity %s is destroying component %s...", component_get_parent_name(this), component_get_key(this));
    if (this->vtable->on_destroy != NULL) {
        this->vtable->on_destroy(this);
    }
    free(this);
}

struct entity* component_get_parent(const struct component *this) {
    return this->parent;
}
void component_set_parent(struct component *this, struct entity *parent) {
    this->parent = parent;
}
const char* component_get_parent_name(const struct component *this) {
    return this->parent != NULL ? entity_get_name(this->parent) : "<none>";
}

struct vector2 component_get_local_position(const struct component *this) {
    return this->local_position;
}
void component_set_local_position(struct component *this, const struct vector2 *local_position) {
    this->local_position = *local_position;
}
struct vector2 component_get_local_scale(const struct component *this) {
    return this->local_scale;
}
void component_set_local_scale(struct component *this, const struct vector2 *local_scale) {
    this->local_scale = *local_scale;
}

struct vector2 component_get_position(const struct component *this) {
    if (this->parent == NULL) {
        return this->local_position;
    }
    const struct vector2 parent_position = transform_get_position(entity_get_transform(this->parent));
    return vector_sum(&parent_position, &this->local_position);
}
struct vector2 component_get_scale(const struct component *this) {
    if (this->parent == NULL) {
        return this->local_scale;
    }
    const struct vector2 parent_scale = transform_get_scale(entity_get_transform(this->parent));
    return vector_multiply(&parent_scale, &this->local_scale);
}

const char* component_get_key(const struct component *this) {
    return this->vtable->component_key();
}

void component_update(struct component *this, const struct update_context *context) {
    if (this->vtable->on_update != NULL) {
        this->vtable->on_update(this, context);
    }
}
