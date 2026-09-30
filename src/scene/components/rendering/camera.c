#include "camera.h"

#include <stdlib.h>

#include "../core/transform.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../chunks/chunks.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/parser.h"


struct camera {
    struct component base;

    double simulation_distance;

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
    component_base_create(base, &camera_vtable, parser, parent);

    parser_next_double(parser, &this->simulation_distance);

    return base;
}

static struct component* camera_clone(struct component base, const struct component *component) {
    struct camera *camera = (struct camera *) component;

    struct camera* this = calloc(1, sizeof(struct camera));
    this->base = base;
    this->simulation_distance = camera->simulation_distance;
    return (struct component*)this;
}

static void camera_awake(struct component *base) {
    struct camera *this = (struct camera *) base;

    const struct scene* scene = entity_get_parent(component_get_parent(base));
    this->renderer = renderer_get_pipeline((struct renderer*)subsystem_collection_get(scene_get_subsystems(scene), "renderer"));
    this->chunks = scene_get_chunks(scene);
}

static void camera_update_renderer_pipeline_viewport(const struct camera *this) {
    renderer_pipeline_set_viewpoint(this->renderer, component_get_rect((struct component*)this).position);
}

static void camera_update_chunks(const struct camera *this, const struct rect rect, bool (*validate)(const struct component* component), void (*execute)(struct component* component, const struct update_context* context), const struct update_context *context) {
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(this->chunks, rect, &x_start, &x_end, &y_start, &y_end);
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
                    if (!validate(component)) {
                        break;
                    }
                    execute(component, context);
                }
            }
        }
    }
}

static void camera_update_visibility_chunks(const struct camera *this, const struct update_context *context) {
    camera_update_chunks(this, renderer_pipeline_get_viewport(this->renderer), component_is_visible_chunkable, component_visible_chunk_update, context);
}

static void camera_update_simulation_chunks(const struct camera *this, const struct update_context *context) {
    const struct rect rect = {
        .position = component_get_rect((const struct component*)this).position,
        .size.x = this->simulation_distance,
        .size.y = this->simulation_distance
    };
    camera_update_chunks(this, rect, component_is_simulation_chunkable, component_simulation_chunk_update, context);
}

static void camera_update(struct component *base, const struct update_context *context) {
    const struct camera *this = (struct camera *) base;
    camera_update_renderer_pipeline_viewport(this);
    camera_update_visibility_chunks(this, context);
    camera_update_simulation_chunks(this, context);
}

static void camera_on_disable(struct component *base) {
    const struct camera *this = (struct camera *) base;

    renderer_pipeline_remove_viewport(this->renderer);
}
