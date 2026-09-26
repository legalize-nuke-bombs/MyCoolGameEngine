//
// Created by Nikita on 26.09.2026.
//

#include "light_map.h"

#include <stdlib.h>
#include <SDL3/SDL.h>
#include "../../logging/logger.h"
#include "../renderer_primitive_internal.h"


#define DRAW_CALLS_BUFFER_SIZE (1 << 10)


struct light_map {
    struct renderer_primitive base;

    struct color darkness_color;
    SDL_Texture *darkness_mask;
    SDL_BlendMode light_source_blend_mode;

    struct light_map_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;
};

static void light_map_on_destroy(struct renderer_primitive* base);
static void light_map_draw(struct renderer_primitive* base, struct rect rect, struct rect viewport, SDL_Renderer* renderer);

static const struct rendering_primitive_vtable light_map_vtable = {
    .on_destroy = light_map_on_destroy,
    .draw = light_map_draw
};

struct renderer_primitive* light_map_create() {
    logger_info("Light map is creating...");
    struct light_map* this = calloc(1, sizeof(struct light_map));
    this->base.vtable = &light_map_vtable;

    light_map_reset(this);
    this->light_source_blend_mode = SDL_ComposeCustomBlendMode(
       SDL_BLENDFACTOR_ZERO,
       SDL_BLENDFACTOR_ONE,
       SDL_BLENDOPERATION_ADD,
       SDL_BLENDFACTOR_ZERO,
       SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
       SDL_BLENDOPERATION_ADD
   );

    return (struct renderer_primitive*)this;
}

void light_map_on_destroy(struct renderer_primitive* base) {
    logger_info("Light map is destroying...");
    struct light_map* this = (struct light_map*)base;
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
        this->darkness_mask = NULL;
    }
}

static void light_map_sync_darkness_size(struct light_map* this, SDL_Renderer *native_renderer) {
    int new_w, new_h;
    SDL_GetCurrentRenderOutputSize(native_renderer, &new_w, &new_h);

    if (this->darkness_mask != NULL) {
        float old_w, old_h;
        SDL_GetTextureSize(this->darkness_mask, &old_w, &old_h);

        if ((int)old_w == new_w && (int)old_h == new_h) {
            return;
        }
    }

    logger_info("Light map is resizing...");
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
    }
    this->darkness_mask = SDL_CreateTexture(native_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, new_w, new_h);
    SDL_SetTextureBlendMode(this->darkness_mask, SDL_BLENDMODE_BLEND);
}

void light_map_draw_primitive(struct light_map* this, const struct light_map_draw_call draw_call) {
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE) {
        logger_warn("Light map buffer size overflow");
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

static void light_map_draw(struct renderer_primitive* base, const struct rect rect, const struct rect viewport, SDL_Renderer* native_renderer) {
    struct light_map* this = (struct light_map*)base;

    light_map_sync_darkness_size(this, native_renderer);

    SDL_Texture* original_target = SDL_GetRenderTarget(native_renderer);
    SDL_SetRenderTarget(native_renderer, this->darkness_mask);

    SDL_SetRenderDrawColor(native_renderer, this->darkness_color.r, this->darkness_color.g, this->darkness_color.b, this->darkness_color.a);
    SDL_RenderClear(native_renderer);

    SDL_BlendMode original_blend_mode;
    SDL_GetRenderDrawBlendMode(native_renderer, &original_blend_mode);
    SDL_SetRenderDrawBlendMode(native_renderer, this->light_source_blend_mode);

    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct light_map_draw_call draw_call = this->draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, viewport, native_renderer);
    }
    this->draw_calls_count = 0;

    SDL_SetRenderDrawBlendMode(native_renderer, original_blend_mode);

    SDL_SetRenderTarget(native_renderer, original_target);

    SDL_RenderTexture(native_renderer, this->darkness_mask, NULL, NULL);
}

void light_map_reset(struct light_map* this) {
    light_map_reset_darkness_color(this);
}

void light_map_set_darkness_color(struct light_map* this, const struct color color) {
    this->darkness_color = color;
}
void light_map_reset_darkness_color(struct light_map* this) {
    this->darkness_color = (struct color){
        .r = 20,
        .g = 20,
        .b = 20,
        .a = 100
    };
}