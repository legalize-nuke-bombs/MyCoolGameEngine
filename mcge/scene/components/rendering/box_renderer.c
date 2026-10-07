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
#include "../../../random/random.h"
#include "../../../rendering/renderer.h"
#include "../../../rendering/renderer_layers.h"
#include "../../../rendering/texture.h"
#include "../../../rendering/textures.h"
#include "../../../utils/fields.h"

struct box_renderer {
    struct component base;

    struct renderer_pipeline* renderer;

    char* renderer_layer_name;
    struct renderer_layer* renderer_layer;

    char* texture_name;
    int texture_frame;
    struct renderer_primitive* square;
};

static void box_renderer_on_create(struct component *base, struct fields *fields);
static void box_renderer_awake(struct component *base);
static void box_renderer_visible_chunk_update(struct component *base, const struct update_context *context);
static void box_renderer_on_destroy(struct component *base);

const struct component_vtable box_renderer_vtable = {
    .component_key = box_renderer_component_key,
    .size = sizeof(struct box_renderer),
    .on_create = box_renderer_on_create,
    .on_awake = box_renderer_awake,
    .on_visible_chunk_update = box_renderer_visible_chunk_update,
    .on_destroy = box_renderer_on_destroy
};

const char* box_renderer_component_key(void) {
    return "box_renderer";
}

static void box_renderer_on_create(struct component *base, struct fields *fields) {
    struct box_renderer *this = (struct box_renderer*) base;

    this->renderer_layer_name = fields_dup_string(fields, "layer", NULL);

    if (fields_has(fields, "texture")) {
        const struct fields_list *textures = fields_get_list(fields, "texture");
        const int count = fields_list_count(textures);
        if (count > 0) {
            const int index = count > 1 ? random_next_int(0, count) : 0;
            this->texture_name = strdup(fields_key(fields_list_get(textures, index)));
        }
        this->texture_frame = fields_get_int(fields, "frame", 0);
    }
    else {
        if (!fields_has(fields, "color")) {
            logger_warn("Box renderer expected field `texture` or `color`");
        }
        this->square = renderer_square_create_from_color(fields_get_color(fields, "color", color_black));
    }
}
static void box_renderer_on_destroy(struct component *base) {
    const struct box_renderer *this = (struct box_renderer *) base;
    if (this->square != NULL) {
        renderer_primitive_destroy(this->square);
    }
    free(this->renderer_layer_name);
    free(this->texture_name);
}

static void box_renderer_awake(struct component *base) {
    struct box_renderer *this = (struct box_renderer *) base;

    this->renderer = renderer_get_pipeline();

    if (this->renderer_layer == NULL) {
        this->renderer_layer = renderer_layers_try_get(this->renderer_layer_name);
    }
    free(this->renderer_layer_name);
    this->renderer_layer_name = NULL;

    if (this->square == NULL) {
        struct texture* texture = textures_get(this->texture_name);
        if (texture == NULL) {
            this->square = renderer_square_create_from_color(color_black);
        }
        else {
            this->square = renderer_square_create_from_texture(texture, this->texture_frame);
        }
    }
    free(this->texture_name);
    this->texture_name = NULL;
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

void box_renderer_bump_texture_frame(const struct box_renderer *this) {
    renderer_square_bump_texture_frame((struct renderer_square*)this->square);
}
int box_renderer_get_texture_frames(const struct box_renderer *this) {
    return renderer_square_get_texture_frames((struct renderer_square*)this->square);
}