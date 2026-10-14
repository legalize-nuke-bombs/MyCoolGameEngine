//
// Created by nikita on 14.10.2026.
//

#include "renderer_text.h"

#include "mcge/rendering/primitives/renderer_primitive_internal.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "mcge/modules/text/text_module.h"
#include "mcge/modules/text/fonts/font.h"


struct renderer_text {
    struct renderer_primitive base;
    TTF_Text *text;
};


static void renderer_text_draw(struct renderer_primitive* base, struct rect rect, struct rect viewport, struct vector2 output_size, SDL_Renderer* renderer);
static bool renderer_text_is_visible(struct renderer_primitive* base, struct rect rect, struct rect viewport);
static void renderer_text_on_destroy(struct renderer_primitive* base);


static const struct rendering_primitive_vtable renderer_text_vtable = {
    .draw = renderer_text_draw,
    .is_visible = renderer_text_is_visible,
    .on_destroy = renderer_text_on_destroy
};


struct renderer_primitive* renderer_text_create_from_string(const char *string, struct font *font) {
    struct renderer_text* this = calloc(1, sizeof(struct renderer_text));
    this->base.vtable = &renderer_text_vtable;
    this->text = TTF_CreateText(text_module_get_native_text_engine(), font_get_native_font(font), string, strlen(string));
    return (struct renderer_primitive*)this;
}
static void renderer_text_on_destroy(struct renderer_primitive* base) {
    const struct renderer_text* this = (struct renderer_text*)base;
    TTF_DestroyText(this->text);
}






static void renderer_text_draw(struct renderer_primitive* base, struct rect rect, struct rect viewport, struct vector2 output_size, SDL_Renderer* renderer) {

}



static bool renderer_text_is_visible(struct renderer_primitive* base, struct rect rect, struct rect viewport) {
    return true;
}