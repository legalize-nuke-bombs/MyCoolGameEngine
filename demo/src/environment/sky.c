//
// Created by Nikita on 01.10.2026.
//

#include "sky.h"

#include <math.h>
#include <stdlib.h>

#include "clock.h"
#include "scene/components/component_internal.h"
#include "scene/scene.h"
#include "scene/entity.h"
#include "logging/logger.h"
#include "utils/dictionary.h"
#include "utils/color.h"
#include "utils/fields.h"
#include "rendering/renderer.h"
#include "rendering/renderer_pipeline.h"
#include "rendering/primitives/custom/light_map.h"

struct sky {
    struct component base;

    struct color day_color;
    struct color night_color;

    struct clock *clock;
    struct light_map *light_map;
};

static void sky_on_create(struct component *base, struct fields *fields);
static void sky_awake(struct component *base);
static void sky_on_disable(struct component *base);
static void sky_update(struct component* base, const struct update_context *context);

const struct component_vtable sky_vtable = {
    .component_key = sky_component_key,
    .size = sizeof(struct sky),
    .on_create = sky_on_create,
    .on_awake = sky_awake,
    .on_disable = sky_on_disable,
    .on_update = sky_update
};

const char* sky_component_key(void) {
    return "sky";
}

static void sky_on_create(struct component *base, struct fields *fields) {
    struct sky *this = (struct sky *) base;
    this->day_color = fields_get_color(fields, "day", color_white);
    this->night_color = fields_get_color(fields, "night", color_black);
}

static void sky_awake(struct component *base) {
    struct sky* this = (struct sky*)base;

    const struct dictionary* clocks = tmap_try_get_components(scene_get_tmap(), "clock");
    if (clocks == NULL) {
        logger_error("Entity %s failed to find clock", component_get_global_parent_name(base));
        entity_mark_destroyed(component_get_parent(base));
        return;
    }
    struct dictionary_iterator iterator = dictionary_begin(clocks);
    struct dictionary_node node;
    if (dictionary_next(clocks, &iterator, &node)) {
        this->clock = node.value;
    }
    else {
        logger_error("Entity %s failed to find clock", component_get_global_parent_name(base));
        entity_mark_destroyed(component_get_parent(base));
        return;
    }

    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline());
}
static void sky_on_disable(struct component *base) {
    struct sky* this = (struct sky*)base;
    this->clock = NULL;
    this->light_map = NULL;
}

static void sky_update(struct component* base, const struct update_context *context) {
    const struct sky* this = (struct sky*)base;

    const double cycle_progress = clock_get_cycle_progress(this->clock);
    const double day_factor = 1.0 - fabsl((cycle_progress - 0.5) * 2.0);
    const double night_factor = 1 - day_factor;
    const struct color output_color = {
        .r = this->day_color.r * day_factor + this->night_color.r * night_factor,
        .g = this->day_color.g * day_factor + this->night_color.g * night_factor,
        .b = this->day_color.b * day_factor + this->night_color.b * night_factor,
        255
    };

    light_map_set_darkness_color(this->light_map, output_color);
}