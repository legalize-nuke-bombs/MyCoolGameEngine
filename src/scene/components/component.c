#include "component_internal.h"

#include <stdlib.h>
#include <string.h>

#include "../entity.h"
#include "../../logging/logger.h"
#include "../../utils/vector2_math.h"
#include "core/transform.h"
#include "../../utils/parser.h"

void component_base_create(struct component *this, const struct component_vtable *vtable, struct parser *parser, struct entity *parent) {
    this->vtable = vtable;
    logger_debug("Component %s is creating...", component_get_key(this));
    this->awake = false;
    this->alive = true;

    const char* word = parser_next(parser);
    if (strcmp(word, "default") == 0) {
        this->local_position = vector2_zero;
        this->local_scale = vector2_one;
    }
    else if (strcmp(word, "custom") == 0) {
        parser_next_double(parser, &this->local_position.x);
        parser_next_double(parser, &this->local_position.y);
        parser_next_double(parser, &this->local_scale.x);
        parser_next_double(parser, &this->local_scale.y);
    }
    else {
        logger_warn("Unexpected component local transform start token `%s`", word);
    }
    this->parent = parent;
}
struct component* component_clone(const struct component *component) {
    struct component this;
    this.vtable = component->vtable;
    logger_debug("Component %s is cloning...", component_get_key(component));
    this.awake = false;
    this.alive = true;
    this.local_position = component->local_position;
    this.local_scale = component->local_scale;
    return component->vtable->on_clone(this, component);
}
void component_awake(struct component *this) {
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
    return this->parent != NULL ? entity_get_name(this->parent) : "<none>";
}

struct vector2 component_get_local_position(const struct component *this) {
    return this->local_position;
}
struct vector2 component_get_local_scale(const struct component *this) {
    return this->local_scale;
}

struct vector2 component_get_position(const struct component *this) {
    if (this->parent == NULL) {
        return this->local_position;
    }
    const struct vector2 parent_position = transform_get_position(entity_get_transform(this->parent));
    const struct vector2 parent_scale = transform_get_scale(entity_get_transform(this->parent));
    return vector_sum(parent_position, vector_multiply_vector(this->local_position, parent_scale));
}
struct vector2 component_get_scale(const struct component *this) {
    if (this->parent == NULL) {
        return this->local_scale;
    }
    const struct vector2 parent_scale = transform_get_scale(entity_get_transform(this->parent));
    return vector_multiply_vector(parent_scale, this->local_scale);
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
