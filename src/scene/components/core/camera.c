#include "camera.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../chunks/chunks.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/parser.h"


struct camera {
    struct component base;

    double visible_height;

    struct renderer_pipeline *renderer;
    const struct chunks *chunks;
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
    component_base_create(base, &camera_vtable, parent);

    parser_next_double(parser, &this->visible_height);

    return base;
}

static struct component* camera_clone(struct component base, const struct component *component) {
    struct camera *camera = (struct camera *) component;

    struct camera* this = calloc(1, sizeof(struct camera));
    this->base = base;
    this->visible_height = camera->visible_height;
    return (struct component*)this;
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
