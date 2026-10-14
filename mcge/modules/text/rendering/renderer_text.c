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
#include "mcge/utils/rect_math.h"


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


struct renderer_primitive* renderer_text_create_from_string(const char *string, struct color color, struct font *font) {
    struct renderer_text* this = calloc(1, sizeof(struct renderer_text));
    this->base.vtable = &renderer_text_vtable;
    this->text = TTF_CreateText(text_module_get_native_text_engine(), font_get_native_font(font), string, strlen(string));
    TTF_SetTextColor(this->text, color.r, color.g, color.b, color.a);
    return (struct renderer_primitive*)this;
}
static void renderer_text_on_destroy(struct renderer_primitive* base) {
    const struct renderer_text* this = (struct renderer_text*)base;
    TTF_DestroyText(this->text);
}






static void renderer_text_draw(struct renderer_primitive* base, const struct rect rect, const struct rect viewport, const struct vector2 output_size, SDL_Renderer* renderer) {
    struct renderer_text* this = (struct renderer_text*)base;

    const struct rect target_rect = rect_sdl(&rect, &viewport, output_size);
    const SDL_FRect sdl_target_rect = {
        .x = target_rect.position.x,
        .y = target_rect.position.y,
        .w = target_rect.size.x,
        .h = target_rect.size.y
    };

    TTF_DrawRendererText(this->text, sdl_target_rect.x, sdl_target_rect.y);
}



static bool renderer_text_is_visible(struct renderer_primitive* base, const struct rect rect, const struct rect viewport) {
    return rects_intersection(rect, viewport);
}

void renderer_text_set_string(const struct renderer_text *this, const char *string) {
    TTF_SetTextString(this->text, string, strlen(string));
}