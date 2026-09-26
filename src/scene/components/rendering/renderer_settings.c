//
// Created by Nikita on 26.09.2026.
//

#include "renderer_settings.h"

#include <stddef.h>
#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../scene/scene.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/light_map/light_map.h"
#include "../../../utils/parser.h"


struct renderer_settings {
    struct component base;

    struct color darkness_color;

    struct light_map *light_map;
};

static void renderer_settings_awake(struct component *base);
static void renderer_settings_on_disable(struct component *base);

static const struct component_vtable renderer_settings_vtable = {
    .component_key = renderer_settings_component_key,
    .on_awake = renderer_settings_awake,
    .on_update = NULL,
    .on_disable = renderer_settings_on_disable
};

const char* renderer_settings_component_key(void) {
    return "renderer_settings";
}

struct component* renderer_settings_create(struct parser *parser, struct entity *parent) {
    struct renderer_settings *this = calloc(1, sizeof(struct renderer_settings));
    struct component *base = (struct component *) this;
    component_create(base, &renderer_settings_vtable, parser, parent);

    parser_next_uint8(parser, &this->darkness_color.r);
    parser_next_uint8(parser, &this->darkness_color.g);
    parser_next_uint8(parser, &this->darkness_color.b);
    parser_next_uint8(parser, &this->darkness_color.a);

    return base;
}

static void renderer_settings_awake(struct component *base) {
    struct renderer_settings *this = (struct renderer_settings *) base;

    const struct renderer* renderer_subsystem = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "renderer");
    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline(renderer_subsystem));
    light_map_set_darkness_color(this->light_map, this->darkness_color);
}

static void renderer_settings_on_disable(struct component *base) {
    const struct renderer_settings *this = (struct renderer_settings *)base;
    light_map_reset_darkness_color(this->light_map);
}