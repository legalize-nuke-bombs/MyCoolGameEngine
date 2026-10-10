#include "camera.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../chunks/chunks_algorithms.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/fields.h"


struct camera {
    struct component base;

    double visible_height;

    struct renderer_pipeline *renderer;
    const struct chunks *chunks;
};

static void camera_on_create(struct component *base, struct fields *fields);
static void camera_awake(struct component *base);
static void camera_update(struct component *base, const struct update_context *context);
static void camera_on_disable(struct component *base);

const struct component_vtable camera_vtable = {
    .component_key = camera_component_key,
    .size = sizeof(struct camera),
    .on_create = camera_on_create,
    .on_awake = camera_awake,
    .on_update = camera_update,
    .on_disable = camera_on_disable
};

const char* camera_component_key(void) {
    return "camera";
}

static void camera_on_create(struct component *base, struct fields *fields) {
    struct camera *this = (struct camera *) base;
    this->visible_height = fields_get_double(fields, "height", 0);
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;

    this->renderer = renderer_get_pipeline();
    this->chunks = scene_get_chunks();
}

static void camera_update_renderer_pipeline_viewport(const struct camera *this) {
    const struct vector2 output_size = renderer_pipeline_get_output_size(this->renderer);
    if (output_size.y <= 0 || this->visible_height <= 0) {
        renderer_pipeline_remove_viewport(this->renderer);
        return;
    }
    const double pixels_per_meter = output_size.y / this->visible_height;

    const struct rect viewport = {
        .position = component_get_rect((struct component*)this).position,
        .size = { output_size.x / pixels_per_meter, this->visible_height }
    };
    renderer_pipeline_set_viewport(this->renderer, viewport);
}

static void camera_update_visible_component(struct component* component, void *context) {
    const struct update_context *update_context = context;
    component_visible_chunk_update(component, update_context);
}

static void camera_update_visible_chunks(const struct camera *this, const struct update_context *context) {
    chunks_algorithms_for_each(renderer_pipeline_get_viewport(this->renderer), component_is_visible_chunkable, camera_update_visible_component, (void*)context);
}

static void camera_update(struct component *base, const struct update_context *context) {
    const struct camera *this = (struct camera *) base;
    camera_update_renderer_pipeline_viewport(this);
    camera_update_visible_chunks(this, context);
}

static void camera_on_disable(struct component *base) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_remove_viewport(this->renderer);
}
