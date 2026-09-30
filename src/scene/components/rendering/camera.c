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


struct camera {
    struct component base;

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

    const struct scene* scene = entity_get_parent(component_get_parent(base));
    this->renderer = renderer_get_pipeline((struct renderer*)subsystem_collection_get(scene_get_subsystems(scene), "renderer"));
    this->chunks = scene_get_chunks(scene);
}

static void camera_update_renderer_pipeline_viewport(const struct camera *this) {
    renderer_pipeline_set_viewpoint(this->renderer, component_get_rect((struct component*)this).position);
}

static void camera_update_visible_chunks(const struct camera *this, const struct update_context *context) {
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(this->chunks, renderer_pipeline_get_viewport(this->renderer), &x_start, &x_end, &y_start, &y_end);
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *dict = chunks_chunk_get_components(this->chunks, x, y);
            if (dict == NULL) {
                continue;
            }
            struct dictionary_iterator dict_iterator = dictionary_begin(dict);
            struct dictionary_node dict_node;
            while (dictionary_next(dict, &dict_iterator, &dict_node)) {
                struct component* component = dict_node.value;
                if (component == NULL) {
                    continue;
                }
                component_visible_chunk_update(component, context);
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
