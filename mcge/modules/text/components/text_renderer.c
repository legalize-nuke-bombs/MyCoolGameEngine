//
// Created by nikita on 14.10.2026.
//

#include "text_renderer.h"

#include <stdlib.h>

#include "mcge/modules/text/fonts/fonts.h"
#include "mcge/modules/text/rendering/renderer_text.h"
#include "mcge/rendering/renderer.h"
#include "mcge/rendering/renderer_layers.h"
#include "mcge/rendering/renderer_pipeline.h"
#include "mcge/rendering/primitives/renderer_primitive.h"
#include "mcge/scene/components/component_internal.h"
#include "mcge/utils/fields.h"


struct text_renderer {
    struct component base;

    struct renderer_layer *layer;
    struct renderer_text *text;
};


const char* text_renderer_component_key(void) {
    return "text_renderer";
}

static void text_renderer_on_create(struct component* base, struct fields *fields) {
    struct text_renderer* this = (struct text_renderer*)base;

    const char* layer_name = fields_get_string(fields, "layer", NULL);
    this->layer = renderer_layers_try_get(layer_name);

    const char *string = fields_dup_string(fields, "string", NULL);
    const struct color color = fields_get_color(fields, "color", color_white);
    const char* font_name = fields_get_string(fields, "font", NULL);
    struct font *font = fonts_get(font_name);
    this->text = (struct renderer_text*)renderer_text_create_from_string(string, color, font);
}
static void text_renderer_on_destroy(struct component* base) {
    const struct text_renderer* this = (struct text_renderer*)base;
    renderer_primitive_destroy((struct renderer_primitive*)this->text);
}


static void text_renderer_on_visible_chunk_update(struct component* base, const struct update_context *context) {
    const struct text_renderer* this = (struct text_renderer*)base;
    const struct renderer_pipeline_draw_call draw_call = {
        .rect = component_get_rect(base),
        .primitive = (struct renderer_primitive*)this->text,
        .layer = this->layer
    };
    renderer_pipeline_draw_primitive(renderer_get_pipeline(), draw_call);
}


const struct component_vtable text_renderer_vtable = {
    .component_key = text_renderer_component_key,
    .size = sizeof(struct text_renderer),
    .on_create = text_renderer_on_create,
    .on_destroy = text_renderer_on_destroy,
    .on_visible_chunk_update = text_renderer_on_visible_chunk_update,
};