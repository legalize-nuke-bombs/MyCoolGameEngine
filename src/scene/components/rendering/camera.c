#include "camera.h"

#include <stdlib.h>

#include "../core/transform.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../subsystems/subsystem_collection.h"


struct camera {
    struct component base;

    struct renderer_pipeline *renderer;
};

static struct component* camera_clone(struct component base, const struct component *component);
static void camera_awake(struct component *base);
static void camera_update(struct component *base, const struct update_context *context);
static void camera_on_disable(struct component *base);

static const struct component_vtable camera_vtable = {
    .component_key = camera_component_key,
    .on_clone = camera_clone,
    .on_awake = camera_awake,
    .on_update = camera_update,
    .on_disable = camera_on_disable
};

const char* camera_component_key(void) {
    return "camera";
}

struct component* camera_create(struct parser *parser, struct entity *parent) {
    struct camera *this = calloc(1, sizeof(struct camera));
    struct component *base = (struct component *) this;
    component_base_create(base, &camera_vtable, parser, parent);

    return base;
}

static struct component* camera_clone(struct component base, const struct component *component) {
    struct camera *camera = (struct camera *) component;

    struct camera* this = calloc(1, sizeof(struct camera));
    this->base = base;
    return (struct component*)this;
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;

    this->renderer = renderer_get_pipeline((struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "renderer"));
}

static void camera_update(struct component *base, const struct update_context *context) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_set_viewpoint(this->renderer, component_get_rect(base).position);
}

static void camera_on_disable(struct component *base) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_remove_viewport(this->renderer);
}
