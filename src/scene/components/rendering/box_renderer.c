//
// Created by nikita on 23.09.2026.
//

#include "box_renderer.h"

#include <stdlib.h>
#include <string.h>

#include "../component_internal.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../rendering/primitives/custom/renderer_square.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../logging/logger.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/layers/renderer_layer_manager.h"
#include "../../../rendering/textures/texture_manager.h"
#include "../../../utils/parser.h"
#include "../../../subsystems/subsystem_collection.h"

struct box_renderer {
    struct component base;

    struct renderer_pipeline* renderer;

    char* renderer_layer_name;
    struct renderer_layer* renderer_layer;

    struct renderer_primitive* square;
};

static struct component* box_renderer_clone(struct component base, const struct component *component);
static void box_renderer_awake(struct component *base);
static void box_renderer_visible_chunk_update(struct component *base, const struct update_context *context);
static void box_renderer_on_destroy(struct component *base);

static const struct component_vtable box_renderer_vtable = {
    .component_key = box_renderer_component_key,
    .on_clone = box_renderer_clone,
    .on_awake = box_renderer_awake,
    .on_visible_chunk_update = box_renderer_visible_chunk_update,
    .on_destroy = box_renderer_on_destroy
};

const char* box_renderer_component_key(void) {
    return "box_renderer";
}

struct component* box_renderer_create(struct parser *parser, struct entity *parent) {
    struct box_renderer *this = malloc(sizeof(struct box_renderer));
    struct component *base = (struct component*) this;
    component_base_create(base, &box_renderer_vtable, parser, parent);

    this->renderer = NULL;

    this->renderer_layer_name = parser_next_dup(parser);
    this->renderer_layer = NULL;

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
        const char* texture_name = parser_next(parser);
        struct renderer* renderer = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "renderer");
        struct texture* texture = texture_manager_try_get_texture(renderer_get_texture_manager(renderer), texture_name);
        int texture_frame;
        parser_next_int(parser, &texture_frame);
        if (texture == NULL) {
            logger_warn("Box renderer failed to find specified texture `%s`", texture_name);
            this->square = renderer_square_create_from_color(color_black);
        }
        else {
            this->square = renderer_square_create_from_texture(texture, texture_frame);
        }
    }
    else {
        logger_warn("Box renderer unexpected type token `%s`", type);
        this->square = renderer_square_create_from_color(color_black);
    }


    return base;
}
static struct component* box_renderer_clone(struct component base, const struct component *component) {
    const struct box_renderer *box_renderer = (struct box_renderer*)component;

    struct box_renderer *this = calloc(1, sizeof(struct box_renderer));
    this->base = base;
    this->renderer_layer_name = strdup(box_renderer->renderer_layer_name);
    this->square = renderer_square_clone((struct renderer_square*)box_renderer->square);
    return (struct component*)this;
}

static void box_renderer_on_destroy(struct component *base) {
    const struct box_renderer *this = (struct box_renderer *) base;
    renderer_primitive_destroy(this->square);
    free(this->renderer_layer_name);
}

static void box_renderer_awake(struct component *base) {
    struct box_renderer *this = (struct box_renderer *) base;

    const struct renderer* renderer_subsystem = (struct renderer*)subsystem_collection_get(scene_get_subsystems(entity_get_parent(component_get_parent(base))), "renderer");
    this->renderer = renderer_get_pipeline(renderer_subsystem);
    this->renderer_layer = renderer_layer_manager_try_get(renderer_get_layer_manager(renderer_subsystem), this->renderer_layer_name);
}

static void box_renderer_visible_chunk_update(struct component *base, const struct update_context *context) {
    const struct box_renderer *this = (struct box_renderer *) base;

    const struct renderer_pipeline_draw_call draw_call = {
        .rect = component_get_rect(base),
        .primitive = this->square,
        .layer = this->renderer_layer
    };
    renderer_pipeline_draw_primitive(this->renderer, draw_call);
}

void box_renderer_bump_texture_frame(struct box_renderer *this) {
    renderer_square_bump_texture_frame((struct renderer_square*)this->square);
}