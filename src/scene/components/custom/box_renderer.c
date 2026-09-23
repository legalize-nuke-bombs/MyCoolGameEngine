//
// Created by nikita on 23.09.2026.
//

#include "box_renderer.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/custom/renderer_square.h"
#include "../../../rendering/renderer_layers.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../engine/engine_context.h"

struct box_renderer {
    struct component base;
    struct renderer_pipeline* renderer;
    struct renderer_square* square;
};

static const char* box_renderer_component_key(void);
static void box_renderer_awake(struct component *base);
static void box_renderer_update(struct component *base, const struct update_context *context);
static void box_renderer_free(struct component *base);

static const struct component_vtable box_renderer_vtable = {
    .component_key = box_renderer_component_key,
    .on_awake = box_renderer_awake,
    .on_update = box_renderer_update,
    .on_destroy = NULL,
    .on_free = box_renderer_free
};

static const char* box_renderer_component_key(void) {
    return "box_renderer";
}

struct box_renderer* box_renderer_create(struct color color) {
    struct box_renderer *this = malloc(sizeof(struct box_renderer));
    component_init(box_renderer_as_component(this), &box_renderer_vtable);

    this->renderer = NULL;
    this->square = renderer_square_create(color);

    return this;
}

static void box_renderer_awake(struct component *base) {
    struct box_renderer *this = (struct box_renderer *) base;

    this->renderer = scene_get_engine_context(entity_get_parent(component_get_parent(base)))->renderer_pipeline;
}

static void box_renderer_update(struct component *base, const struct update_context *context) {
    const struct box_renderer *this = (struct box_renderer *) base;

    const struct renderer_pipeline_draw_call draw_call = {
        .rect = {
            .position = component_get_position(base),
            .size = component_get_scale(base)
        },
        .primitive = renderer_square_as_renderer_primitive(this->square),
        .layer = renderer_layer_bg1
    };
    renderer_pipeline_draw_primitive(this->renderer, draw_call);
}

static void box_renderer_free(struct component *base) {
    const struct box_renderer *this = (struct box_renderer *) base;

    renderer_square_destroy(this->square);
}

struct component * box_renderer_as_component(struct box_renderer *this) {
    return (struct component*) this;
}
