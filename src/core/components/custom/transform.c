#include "transform.h"

#include <stddef.h>

static struct component_vtable transform_vtable = {
    .on_update = NULL,
    .on_destroy = NULL
};

void transform_init(struct transform *this, const struct vector2* position, const struct vector2* scale) {
    struct component *base = transform_as_component(this);

    component_init(base);
    base->_vtable = &transform_vtable;

    this->position = *position;
    this->scale = *scale;
}

struct component *transform_as_component(struct transform *this) {
    return (struct component *) this;
}
