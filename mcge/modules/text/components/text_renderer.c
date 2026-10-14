//
// Created by nikita on 14.10.2026.
//

#include "text_renderer.h"

#include <stdlib.h>

#include "mcge/modules/text/fonts/fonts.h"
#include "mcge/rendering/renderer_layers.h"
#include "mcge/scene/components/component_internal.h"
#include "mcge/utils/fields.h"


struct text_renderer {
    struct component base;

    struct renderer_layer *layer;
    struct font *font;
    char *string;
};


const char* text_renderer_component_key(void) {
    return "text_renderer";
}

static void text_renderer_on_create(struct component* base, struct fields *fields) {
    struct text_renderer* this = (struct text_renderer*)base;

    const char* layer_name = fields_get_string(fields, "layer", NULL);
    const char* font_name = fields_get_string(fields, "font", NULL);
    this->string = fields_dup_string(fields, "string", NULL);

    this->layer = renderer_layers_try_get(layer_name);
    this->font = fonts_get(font_name);
}
static void text_renderer_on_destroy(struct component* base) {
    const struct text_renderer* this = (struct text_renderer*)base;
    if (this->string) free(this->string);
}


static void text_renderer_on_visible_chunk_update(struct component* base, const struct update_context *context) {
    const struct text_renderer* this = (struct text_renderer*)base;
}


const struct component_vtable text_renderer_vtable = {
    .component_key = text_renderer_component_key,
    .size = sizeof(struct text_renderer),
    .on_create = text_renderer_on_create,
    .on_destroy = text_renderer_on_destroy,
    .on_visible_chunk_update = text_renderer_on_visible_chunk_update,
};