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
        this->local_rect.position = vector2_zero;
        this->local_rect.size = vector2_one;
    }
    else if (strcmp(word, "custom") == 0) {
        parser_next_double(parser, &this->local_rect.position.x);
        parser_next_double(parser, &this->local_rect.position.y);
        parser_next_double(parser, &this->local_rect.size.x);
        parser_next_double(parser, &this->local_rect.size.y);
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
    this.local_rect = component->local_rect;
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

struct rect component_get_local_rect(const struct component *this) {
    return this->local_rect;
}
void component_set_local_rect(struct component *this, struct rect rect) {
    this->local_rect = rect;
}

struct rect component_get_rect(const struct component *this) {
    if (this->parent == NULL) {
        return this->local_rect;
    }
    const struct rect parent_rect = transform_get_rect(entity_get_transform(this->parent));
    const struct rect result = {
        .position = vector_sum(parent_rect.position, vector_multiply_vector(this->local_rect.position, parent_rect.size)),
        .size = vector_multiply_vector(parent_rect.size, this->local_rect.size)
    };
    return result;
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
