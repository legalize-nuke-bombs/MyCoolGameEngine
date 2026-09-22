#include "transform.h"
#include "../../../utils/vector2_math.h"

#include <stdio.h>

static struct component_vtable printer_vtable = {
    .on_update = NULL,
    .on_destroy = NULL
};

void transform_init(struct transform *this, const struct vector2* position, const struct vector2* scale) {
    struct component *base = transform_as_component(this);

    component_init(base);
    base->_vtable = &printer_vtable;

    this->position = *position;
    this->scale = vector_relu(scale);
}

struct component *transform_as_component(struct transform *this) {
    return (struct component *) this;
}
