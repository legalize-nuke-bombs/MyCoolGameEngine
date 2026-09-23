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
#include <SDL3/SDL.h>


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
    .on_destroy = camera_destroy,
    .on_free = NULL
};

static const char* camera_component_key(void) {
    return "camera";
}

struct camera* camera_create(struct entity *parent) {
    struct camera *this = malloc(sizeof(struct camera));
    component_init(camera_as_component(this), &camera_vtable, parent);

    this->renderer = NULL;

    return this;
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;

    this->renderer = engine_context_get_renderer_pipeline(scene_get_engine_context(entity_get_parent(component_get_parent(base))));
    logger_debug("Entity %s awoken its camera on %f %f", component_get_parent_name(base), component_get_position(base).x, component_get_position(base).y);
}

static void camera_update(struct component *base, const struct update_context *context) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_set_viewpoint(this->renderer, component_get_position(base));
}

static void camera_destroy(struct component *base) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_remove_viewport(this->renderer);
}


struct component *camera_as_component(struct camera *this) {
    return (struct component *) this;
}
