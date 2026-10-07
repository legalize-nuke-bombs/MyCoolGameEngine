//
// Created by Nikita on 26.09.2026.
//

#include "renderer_settings.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../utils/fields.h"
#include "../../../utils/color.h"
#include "../../../rendering/primitives/custom/light_map.h"


struct renderer_settings {
    struct component base;

    uint8_t light_map_enabled;
    struct color light_map_darkness_color;
    struct light_map *light_map;
};

static void renderer_settings_on_create(struct component *base, struct fields *fields);
static void renderer_settings_awake(struct component *base);
static void renderer_settings_on_disable(struct component *base);

const struct component_vtable renderer_settings_vtable = {
    .component_key = renderer_settings_component_key,
    .size = sizeof(struct renderer_settings),
    .on_create = renderer_settings_on_create,
    .on_awake = renderer_settings_awake,
    .on_update = NULL,
    .on_disable = renderer_settings_on_disable
};

const char* renderer_settings_component_key(void) {
    return "renderer_settings";
}

static void renderer_settings_on_create(struct component *base, struct fields *fields) {
    struct renderer_settings *this = (struct renderer_settings *) base;
    this->light_map_enabled = fields_has(fields, "darkness");
    this->light_map_darkness_color = fields_get_color(fields, "darkness", color_black);
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