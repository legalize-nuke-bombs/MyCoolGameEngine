#include "camera.h"

#include <stddef.h>

static const char* camera_component_key(void);

static struct component_vtable camera_vtable = {
    .component_key = camera_component_key,
    .on_awake = NULL,
    .on_update = NULL,
    .on_destroy = NULL
};

static const char* camera_component_key(void) {
    return "camera";
}

void camera_init(struct camera *this) {
    struct component *base = camera_as_component(this);

    component_create(base);
    base->_vtable = &camera_vtable;
}

struct component *camera_as_component(struct camera *this) {
    return (struct component *) this;
}
