//
// Created by nikita on 26.09.2026.
//

#include "box_light.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/custom/renderer_square.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../rendering/custom/light_map.h"

struct box_light {
    struct component base;

    struct light_map* light_map;

    struct renderer_primitive* square;
};

static void box_light_awake(struct component *base);
static void box_light_update(struct component *base, const struct update_context *context);
static void box_light_on_destroy(struct component *base);

static const struct component_vtable box_light_vtable = {
    .component_key = box_light_component_key,
    .on_awake = box_light_awake,
    .on_update = box_light_update,
    .on_disable = NULL,
    .on_destroy = box_light_on_destroy
};

const char* box_light_component_key(void) {
    return "box_light";
}

struct component* box_light_create(struct parser *parser, struct entity *parent) {
    struct box_light *this = malloc(sizeof(struct box_light));
    struct component *base = (struct component*) this;
    component_create(base, &box_light_vtable, parser, parent);

    this->light_map = NULL;

    const struct color color = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 255
    };
    this->square = renderer_square_create_from_color(color);

    return base;
}

static void box_light_on_destroy(struct component *base) {
    const struct box_light *this = (struct box_light *) base;
    renderer_primitive_destroy(this->square);
}

static void box_light_awake(struct component *base) {
    struct box_light *this = (struct box_light *) base;

    const struct renderer* renderer_subsystem = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "renderer");
    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline(renderer_subsystem));
}

static void box_light_update(struct component *base, const struct update_context *context) {
    const struct box_light *this = (struct box_light *) base;

    const struct light_map_draw_call draw_call = {
        .rect = {
            .position = component_get_position(base),
            .size = component_get_scale(base)
        },
        .primitive = this->square,
    };
    light_map_draw_primitive(this->light_map, draw_call);
}