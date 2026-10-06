#include "camera.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../chunks/chunks.h"
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

static void camera_update_visible_chunks(const struct camera *this, const struct update_context *context) {
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(this->chunks, renderer_pipeline_get_viewport(this->renderer), &x_start, &x_end, &y_start, &y_end);
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *types = chunks_chunk_get_types(this->chunks, x, y);
            if (types == NULL) {
                continue;
            }
            struct dictionary_iterator types_iterator = dictionary_begin(types);
            struct dictionary_node node;
            while (dictionary_next(types, &types_iterator, &node)) {
                const struct dictionary* typed_components = node.value;

                struct dictionary_iterator typed_components_iterator = dictionary_begin(typed_components);
                while (dictionary_next(typed_components, &typed_components_iterator, &node)) {
                    struct component* component = node.value;
                    if (!component_is_visible_chunkable(component)) {
                        break;
                    }
                    if (!component_is_awake(component)) {
                        continue;
                    }
                    component_visible_chunk_update(component, context);
                }
            }
        }
    }
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
