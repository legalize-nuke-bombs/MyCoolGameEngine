//
// Created by nikita on 26.09.2026.
//

#include "box_light.h"

#include <stdlib.h>
#include <string.h>

#include "../component_internal.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/primitives/custom/renderer_square.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../logging/logger.h"
#include "../../../rendering/renderer.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../rendering/primitives/custom/light_map.h"
#include "../../../utils/parser.h"
#include "../../../catalogs/catalogs.h"

struct box_light {
    struct component base;

    struct light_map* light_map;

    struct renderer_primitive* square;
};

static struct component* box_light_clone(struct component base, const struct component *component);
static void box_light_awake(struct component *base);
static void box_light_visible_chunk_update(struct component *base, const struct update_context *context);
static void box_light_on_destroy(struct component *base);

static const struct component_vtable box_light_vtable = {
    .component_key = box_light_component_key,
    .on_clone = box_light_clone,
    .on_awake = box_light_awake,
    .on_visible_chunk_update = box_light_visible_chunk_update,
    .on_destroy = box_light_on_destroy
};

const char* box_light_component_key(void) {
    return "box_light";
}

struct component* box_light_create(struct parser *parser, struct entity *parent) {
    struct box_light *this = malloc(sizeof(struct box_light));
    struct component *base = (struct component*) this;
    component_base_create(base, &box_light_vtable, parent);

    this->light_map = NULL;

    const char* type = parser_next(parser);
    if (strcmp(type, "color") == 0) {
        struct color color;
        parser_next_uint8(parser, &color.r);
        parser_next_uint8(parser, &color.g);
        parser_next_uint8(parser, &color.b);
        parser_next_uint8(parser, &color.a);
        this->square = renderer_square_create_from_color(color);
    }
    else if (strcmp(type, "texture") == 0) {
        const struct catalogs* catalogs = (struct catalogs*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "catalogs");
        struct texture* texture = catalogs_get_item(catalogs, "texture", parser_next(parser));
        int texture_frame;
        parser_next_int(parser, &texture_frame);
        if (texture == NULL) {
            this->square = renderer_square_create_from_color(color_white);
        }
        else {
            this->square = renderer_square_create_from_texture(texture, texture_frame);
        }
    }
    else {
        logger_warn("Box light unexpected type `%s`", type);
        this->square = renderer_square_create_from_color(color_white);
    }

    return base;
}

static struct component* box_light_clone(struct component base, const struct component *component) {
    const struct box_light *box_light = (struct box_light*)component;

    struct box_light *this = calloc(1, sizeof(struct box_light));
    this->base = base;
    this->square = renderer_square_clone((struct renderer_square*)box_light->square);
    return (struct component*)this;
}

static void box_light_on_destroy(struct component *base) {
    const struct box_light *this = (struct box_light *) base;
    renderer_primitive_destroy(this->square);
}

static void box_light_awake(struct component *base) {
    struct box_light *this = (struct box_light *) base;

    const struct renderer* renderer_subsystem = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(component_get_parent(base))), "renderer");
    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline(renderer_subsystem));
}

static void box_light_visible_chunk_update(struct component *base, const struct update_context *context) {
    const struct box_light *this = (struct box_light *) base;

    const struct light_map_draw_call draw_call = {
        .rect = component_get_rect(base),
        .primitive = this->square,
    };
    light_map_draw_primitive(this->light_map, draw_call);
}