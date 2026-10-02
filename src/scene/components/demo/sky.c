//
// Created by Nikita on 01.10.2026.
//

#include "sky.h"

#include <math.h>
#include <stdlib.h>

#include "clock.h"
#include "../component_internal.h"
#include "../../scene.h"
#include "../../entity.h"
#include "../../../logging/logger.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/color.h"
#include "../../../utils/parser.h"
#include "../../../rendering/renderer.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/primitives/custom/light_map.h"

struct sky {
    struct component base;

    struct color day_color;
    struct color night_color;

    struct clock *clock;
    struct light_map *light_map;
};

static struct component* sky_clone(struct component base, const struct component *component);
static void sky_awake(struct component *base);
static void sky_on_disable(struct component *base);
static void sky_update(struct component* base, const struct update_context *context);

static const struct component_vtable sky_vtable = {
    .component_key = sky_component_key,
    .on_clone = sky_clone,
    .on_awake = sky_awake,
    .on_disable = sky_on_disable,
    .on_update = sky_update
};

const char* sky_component_key(void) {
    return "sky";
}

struct component* sky_create(struct parser *parser, struct entity *parent) {
    struct sky *this = calloc(1, sizeof(struct sky));
    struct component *base = (struct component *) this;
    component_base_create(base, &sky_vtable, parent);
    parser_next_uint8(parser, &this->day_color.r);
    parser_next_uint8(parser, &this->day_color.g);
    parser_next_uint8(parser, &this->day_color.b);
    parser_next_uint8(parser, &this->day_color.a);
    parser_next_uint8(parser, &this->night_color.r);
    parser_next_uint8(parser, &this->night_color.g);
    parser_next_uint8(parser, &this->night_color.b);
    parser_next_uint8(parser, &this->night_color.a);
    return base;
}

static struct component* sky_clone(struct component base, const struct component *component) {
    const struct sky *sky = (struct sky *) component;

    struct sky* this = calloc(1, sizeof(struct sky));
    this->base = base;
    this->day_color = sky->day_color;
    this->night_color = sky->night_color;
    return (struct component*)this;
}

static void sky_awake(struct component *base) {
    struct sky* this = (struct sky*)base;

    const struct tmap* tmap = scene_get_tmap(entity_get_scene(component_get_parent(base)));;
    const struct dictionary* clocks = tmap_try_get_components(tmap, "clock");
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

    const struct renderer* renderer_subsystem = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "renderer");
    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline(renderer_subsystem));
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