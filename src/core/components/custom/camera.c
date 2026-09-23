#include "camera.h"

#include <stdlib.h>

#include "transform.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../../logging/logger.h"

struct camera {
    struct component base;

    struct transform *transform;
};

static const char* camera_component_key(void);
static void camera_awake(struct component *base);

static const struct component_vtable camera_vtable = {
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
    this->transform = (struct transform*) entity_get_component(component_get_parent(base), "transform");
    logger_debug("Entity %s awoken its camera on %f %f", component_get_parent_name(base), component_get_position(base).x, component_get_position(base).y);
}

struct camera* camera_create(void) {
    struct camera *this = malloc(sizeof(struct camera));
    component_init(camera_as_component(this), &camera_vtable);

    this->transform = NULL;

    return this;
}

struct component *camera_as_component(struct camera *this) {
    return (struct component *) this;
}
