//
// Created by nikita on 14.10.2026.
//

#include "renderer_text.h"

#include "mcge/rendering/primitives/renderer_primitive_internal.h"
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "mcge/allocator/allocator.h"
#include "mcge/modules/text/text_module.h"
#include "mcge/modules/text/fonts/font.h"
#include "mcge/utils/rect_math.h"

#define RENDERER_TEXT_MAX_HEIGHT 512


struct renderer_text {
    struct renderer_primitive base;
    struct font *font;
    TTF_Text *text;
    int height;
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
    struct renderer_text* this = allocator_new0(1, sizeof(struct renderer_text));
    this->base.vtable = &renderer_text_vtable;
    this->font = font;
    this->text = TTF_CreateText(text_module_get_native_text_engine(), NULL, string, 0);
    TTF_SetTextColor(this->text, color.r, color.g, color.b, color.a);
    return (struct renderer_primitive*)this;
}
static void renderer_text_on_destroy(struct renderer_primitive* base) {
    const struct renderer_text* this = (struct renderer_text*)base;
    TTF_DestroyText(this->text);
}






static void renderer_text_draw(struct renderer_primitive* base, const struct rect rect, const struct rect viewport, const struct vector2 output_size, SDL_Renderer* renderer) {
    struct renderer_text* this = (struct renderer_text*)base;
    if (this->font == NULL || this->text == NULL) {
        return;
    }

    const struct rect target_rect = rect_sdl(&rect, &viewport, output_size);

    int height = (int)(target_rect.size.y + 0.5);
    if (height < 1) {
        return;
    }
    if (height > RENDERER_TEXT_MAX_HEIGHT) {
        height = RENDERER_TEXT_MAX_HEIGHT;
    }
    if (height != this->height) {
        this->height = height;
        TTF_SetTextFont(this->text, font_get_native_font_for_height(this->font, height));
    }

    int text_width, text_height;
    if (!TTF_GetTextSize(this->text, &text_width, &text_height)) {
        return;
    }
    TTF_DrawRendererText(this->text,
        (float)(target_rect.position.x + (target_rect.size.x - text_width) / 2),
        (float)(target_rect.position.y + (target_rect.size.y - text_height) / 2));
}



static bool renderer_text_is_visible(struct renderer_primitive* base, const struct rect rect, const struct rect viewport) {
    return rects_intersection(rect, viewport);
}

void renderer_text_set_string(const struct renderer_text *this, const char *string) {
    TTF_SetTextString(this->text, string, 0);
}
