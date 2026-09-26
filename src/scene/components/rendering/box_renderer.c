//
// Created by nikita on 23.09.2026.
//

#include "box_renderer.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/custom/renderer_square.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_layer_manager.h"
#include "../../../utils/parser.h"
#include "../../../subsystems/subsystem_collection.h"

struct box_renderer {
    struct component base;

    struct renderer_pipeline* renderer;

    char* renderer_layer_name;
    struct renderer_layer* renderer_layer;

    struct renderer_square* square;
};

static void box_renderer_awake(struct component *base);
static void box_renderer_update(struct component *base, const struct update_context *context);
static void box_renderer_on_destroy(struct component *base);

static const struct component_vtable box_renderer_vtable = {
    .component_key = box_renderer_component_key,
    .on_awake = box_renderer_awake,
    .on_update = box_renderer_update,
    .on_disable = NULL,
    .on_destroy = box_renderer_on_destroy
};

const char* box_renderer_component_key(void) {
    return "box_renderer";
}

struct component* box_renderer_create(struct parser *parser, struct entity *parent) {
    struct box_renderer *this = malloc(sizeof(struct box_renderer));
    struct component *base = (struct component*) this;
    component_create(base, &box_renderer_vtable, parser, parent);

    this->renderer = NULL;

    this->renderer_layer_name = parser_next_dup(parser);
    this->renderer_layer = NULL;

    struct color color;
    parser_next_uint8(parser, &color.r);
    parser_next_uint8(parser, &color.g);
    parser_next_uint8(parser, &color.b);
    parser_next_uint8(parser, &color.a);
    this->square = renderer_square_create_from_color(color);

    return base;
}

static void box_renderer_on_destroy(struct component *base) {
    const struct box_renderer *this = (struct box_renderer *) base;
    renderer_square_destroy(this->square);
    free(this->renderer_layer_name);
}

static void box_renderer_awake(struct component *base) {
    struct box_renderer *this = (struct box_renderer *) base;

    const struct renderer* renderer_subsystem = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "renderer");
    this->renderer = renderer_get_pipeline(renderer_subsystem);
    this->renderer_layer = renderer_layer_manager_try_get(renderer_get_layer_manager(renderer_subsystem), this->renderer_layer_name);
}

static void box_renderer_update(struct component *base, const struct update_context *context) {
    const struct box_renderer *this = (struct box_renderer *) base;

    const struct renderer_pipeline_draw_call draw_call = {
        .rect = {
            .position = component_get_position(base),
            .size = component_get_scale(base)
        },
        .primitive = renderer_square_as_renderer_primitive(this->square),
        .layer = this->renderer_layer
    };
    renderer_pipeline_draw_primitive(this->renderer, draw_call);
}
