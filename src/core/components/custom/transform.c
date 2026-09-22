#include "transform.h"

#include <stddef.h>

static const char* transform_component_key(void);

static struct component_vtable transform_vtable = {
    .component_key = transform_component_key,
    .on_update = NULL,
    .on_destroy = NULL
};

static const char* transform_component_key(void) {
    return "transform";
}

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
