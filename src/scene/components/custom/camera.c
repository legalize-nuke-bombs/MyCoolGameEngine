#include "camera.h"

#include <stdlib.h>

#include "../transform.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../../engine/engine.h"
#include "../../scene.h"
#include "../../../rendering/renderer_pipeline.h"
#include <SDL3/SDL.h>


struct camera {
    struct component base;

    struct renderer_pipeline *renderer;
};

static void camera_awake(struct component *base);
static void camera_update(struct component *base, const struct update_context *context);
static void camera_destroy(struct component *base);

static const struct component_vtable camera_vtable = {
    .component_key = camera_component_key,
    .on_awake = camera_awake,
    .on_update = camera_update,
    .on_destroy = camera_destroy
};

const char* camera_component_key(void) {
    return "camera";
}

struct component* camera_create(struct parser *parser, struct entity *parent) {
    struct camera *this = malloc(sizeof(struct camera));
    struct component *base = (struct component *) this;
    component_init(base, &camera_vtable, parser, parent);

    this->renderer = NULL;

    return base;
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;

    this->renderer = engine_get_renderer_pipeline(scene_get_engine(entity_get_parent(component_get_parent(base))));
}

static void camera_update(struct component *base, const struct update_context *context) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_set_viewpoint(this->renderer, component_get_position(base));
}

static void camera_destroy(struct component *base) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_remove_viewport(this->renderer);
}
