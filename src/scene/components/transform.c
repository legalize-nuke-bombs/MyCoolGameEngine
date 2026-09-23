#include "transform.h"

#include <stdlib.h>

#include "component_internal.h"

struct transform {
    struct component base;
    struct vector2 position;
    struct vector2 scale;
};

static const char* transform_component_key(void);

static const struct component_vtable transform_vtable = {
    .component_key = transform_component_key,
    .on_awake = NULL,
    .on_update = NULL,
    .on_destroy = NULL
};

static const char* transform_component_key(void) {
    return "transform";
}

struct transform* transform_create(const struct vector2* position, const struct vector2* scale) {
    struct transform *this = malloc(sizeof(struct transform));
    component_init(transform_as_component(this), &transform_vtable);

    this->position = *position;
    this->scale = *scale;

    return this;
}

struct vector2 transform_get_position(const struct transform *this) {
    return this->position;
}
void transform_set_position(struct transform *this, const struct vector2 *position) {
    this->position = *position;
}
struct vector2 transform_get_scale(const struct transform *this) {
    return this->scale;
}
void transform_set_scale(struct transform *this, const struct vector2 *scale) {
    this->scale = *scale;
}

struct component *transform_as_component(struct transform *this) {
    return (struct component *) this;
}
