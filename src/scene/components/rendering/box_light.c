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
#include "../../../rendering/primitives/custom/light_map.h"
#include "../../../utils/fields.h"
#include "../../../rendering/texture.h"

struct box_light {
    struct component base;

    struct light_map* light_map;

    char* texture_name;
    int texture_frame;
    struct renderer_primitive* square;
};

static void box_light_on_create(struct component *base, struct fields *fields);
static void box_light_awake(struct component *base);
static void box_light_visible_chunk_update(struct component *base, const struct update_context *context);
static void box_light_on_destroy(struct component *base);

const struct component_vtable box_light_vtable = {
    .component_key = box_light_component_key,
    .size = sizeof(struct box_light),
    .on_create = box_light_on_create,
    .on_awake = box_light_awake,
    .on_visible_chunk_update = box_light_visible_chunk_update,
    .on_destroy = box_light_on_destroy
};

const char* box_light_component_key(void) {
    return "box_light";
}

static void box_light_on_create(struct component *base, struct fields *fields) {
    struct box_light *this = (struct box_light*) base;

    if (fields_has(fields, "texture")) {
        this->texture_name = fields_dup_string(fields, "texture", NULL);
        this->texture_frame = fields_get_int(fields, "frame", 0);
    }
    else {
        if (!fields_has(fields, "color")) {
            logger_warn("Box light expected field `texture` or `color`");
        }
        this->square = renderer_square_create_from_color(fields_get_color(fields, "color", color_white));
    }
}

static void box_light_on_destroy(struct component *base) {
    const struct box_light *this = (struct box_light *) base;
    if (this->square != NULL) {
        renderer_primitive_destroy(this->square);
    }
    free(this->texture_name);
}

static void box_light_awake(struct component *base) {
    struct box_light *this = (struct box_light *) base;

    this->light_map = renderer_pipeline_get_light_map(renderer_get_pipeline());

    if (this->square == NULL) {
        struct texture* texture = texture_asset_get(this->texture_name);
        if (texture == NULL) {
            this->square = renderer_square_create_from_color(color_white);
        }
        else {
            this->square = renderer_square_create_from_texture(texture, this->texture_frame);
        }
    }
    free(this->texture_name);
    this->texture_name = NULL;
}

static void box_light_visible_chunk_update(struct component *base, const struct update_context *context) {
    const struct box_light *this = (struct box_light *) base;

    const struct light_map_draw_call draw_call = {
        .rect = component_get_rect(base),
        .primitive = this->square,
    };
    light_map_draw_primitive(this->light_map, draw_call);
}