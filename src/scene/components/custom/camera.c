#include "camera.h"

#include <stdlib.h>

#include "../transform.h"
#include "../component_internal.h"
#include "../../../utils/rect.h"
#include "../../entity.h"
#include "../../../logging/logger.h"
#include "../../../engine/engine_context.h"
#include "../../scene.h"
#include "../../../rendering/renderer_pipeline.h"


struct camera {
    struct component base;

    struct renderer_pipeline *renderer;
};

static const char* camera_component_key(void);
static void camera_awake(struct component *base);
static void camera_update(struct component *base, const struct update_context *context);
static void camera_destroy(struct component *base);

static const struct component_vtable camera_vtable = {
    .component_key = camera_component_key,
    .on_awake = camera_awake,
    .on_update = camera_update,
    .on_destroy = camera_destroy
};

static const char* camera_component_key(void) {
    return "camera";
}

struct camera* camera_create(void) {
    struct camera *this = malloc(sizeof(struct camera));
    component_init(camera_as_component(this), &camera_vtable);

    this->renderer = NULL;

    return this;
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;

    this->renderer = scene_get_engine_context(entity_get_parent(component_get_parent(base)))->renderer_pipeline;
    logger_debug("Entity %s awoken its camera on %f %f", component_get_parent_name(base), component_get_position(base).x, component_get_position(base).y);
}

static void camera_update(struct component *base, const struct update_context *context) {
    const struct camera *this = (struct camera *) base;

    const struct rect viewport = {
        .position = component_get_local_position(base),
        .size = component_get_local_scale(base)
    };
    renderer_pipeline_set_viewport(this->renderer, viewport);
}

static void camera_destroy(struct component *base) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_remove_viewport(this->renderer);
}


struct component *camera_as_component(struct camera *this) {
    return (struct component *) this;
}
