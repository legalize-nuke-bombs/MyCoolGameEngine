#include "camera.h"

#include <stddef.h>

#include "transform.h"
#include "../../entity.h"
#include "../../../logging/logger.h"

static const char* camera_component_key(void);
static void camera_awake(struct component *base);

static struct component_vtable camera_vtable = {
    .component_key = camera_component_key,
    .on_awake = camera_awake,
    .on_update = NULL,
    .on_destroy = NULL
};

static const char* camera_component_key(void) {
    return "camera";
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;
    this->_transform = (struct transform*) entity_get_component(base->_parent, "transform");
    logger_debug("Entity %s awoken its camera on %f %f", component_get_parent_name(base), component_get_position(base).x, component_get_position(base).y);
}

void camera_init(struct camera *this) {
    struct component *base = camera_as_component(this);

    component_create(base);
    base->_vtable = &camera_vtable;

    this->_transform = NULL;
}

struct component *camera_as_component(struct camera *this) {
    return (struct component *) this;
}
