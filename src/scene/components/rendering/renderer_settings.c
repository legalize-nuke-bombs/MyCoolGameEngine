//
// Created by Nikita on 26.09.2026.
//

#include "renderer_settings.h"

#include <stddef.h>
#include <stdlib.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../../scene/scene.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../utils/parser.h"
#include "../../../utils/color.h"
#include "../../../rendering/primitives/custom/light_map.h"


struct renderer_settings {
    struct component base;

    uint8_t light_map_enabled;
    struct color light_map_darkness_color;
    struct light_map *light_map;
};

static struct component* renderer_settings_clone(struct component base, const struct component *component);
static void renderer_settings_awake(struct component *base);
static void renderer_settings_on_disable(struct component *base);

static const struct component_vtable renderer_settings_vtable = {
    .component_key = renderer_settings_component_key,
    .on_clone = renderer_settings_clone,
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
    component_base_create(base, &renderer_settings_vtable, parent);

    parser_next_uint8(parser, &this->light_map_enabled);
    if (this->light_map_enabled) {
        parser_next_uint8(parser, &this->light_map_darkness_color.r);
        parser_next_uint8(parser, &this->light_map_darkness_color.g);
        parser_next_uint8(parser, &this->light_map_darkness_color.b);
        parser_next_uint8(parser, &this->light_map_darkness_color.a);
    }

    return base;
}

static struct component* renderer_settings_clone(struct component base, const struct component *component) {
    const struct renderer_settings *renderer_settings = (struct renderer_settings *) component;

    struct renderer_settings* this = calloc(1, sizeof(struct renderer_settings));
    this->base = base;
    this->light_map_enabled = renderer_settings->light_map_enabled;
    this->light_map_darkness_color = renderer_settings->light_map_darkness_color;
    return (struct component*)this;
}

static void renderer_settings_awake(struct component *base) {
    struct renderer_settings *this = (struct renderer_settings *) base;

    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline());
    light_map_set_enable(this->light_map, this->light_map_enabled);
    if (this->light_map_enabled) {
        light_map_set_darkness_color(this->light_map, this->light_map_darkness_color);
    }
}

static void renderer_settings_on_disable(struct component *base) {
    const struct renderer_settings *this = (struct renderer_settings *)base;
    light_map_reset_darkness_color(this->light_map);
}