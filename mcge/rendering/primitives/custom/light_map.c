//
// Created by Nikita on 26.09.2026.
//

#include "light_map.h"

#include <stdlib.h>
#include <SDL3/SDL.h>
#include "../../../logging/logger.h"
#include "../renderer_primitive_internal.h"


#define DRAW_CALLS_BUFFER_SIZE (1 << 10)


struct light_map {
    struct renderer_primitive base;

    bool enabled;

    struct color darkness_color;
    SDL_Texture *darkness_mask;
    SDL_BlendMode light_source_blend_mode;

    struct light_map_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;
    int dropped_draw_calls_count;
};

static void light_map_draw(struct renderer_primitive* base, struct rect rect, struct rect viewport, struct vector2 output_size, SDL_Renderer* renderer);
static bool light_map_is_visible(struct renderer_primitive* base, struct rect rect, struct rect viewport);
static void light_map_on_destroy(struct renderer_primitive* base);

static const struct rendering_primitive_vtable light_map_vtable = {
    .draw = light_map_draw,
    .is_visible = light_map_is_visible,
    .on_destroy = light_map_on_destroy
};

struct renderer_primitive* light_map_create() {
    logger_info("Light map is creating...");
    struct light_map* this = calloc(1, sizeof(struct light_map));
    this->base.vtable = &light_map_vtable;

    light_map_reset(this);

    this->light_source_blend_mode = SDL_BLENDMODE_ADD;

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
}

void light_map_draw_primitive(struct light_map* this, const struct light_map_draw_call draw_call) {
    if (!this->enabled) {
        return;
    }
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE) {
        this->dropped_draw_calls_count++;
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

void light_map_clear_draw_calls(struct light_map* this) {
    this->draw_calls_count = 0;
    this->dropped_draw_calls_count = 0;
}

static void light_map_draw(struct renderer_primitive* base, const struct rect rect, const struct rect viewport, const struct vector2 output_size, SDL_Renderer* native_renderer) {
    struct light_map* this = (struct light_map*)base;
    if (!this->enabled) {
        return;
    }
    if (this->dropped_draw_calls_count > 0) {
        logger_warn("Light map buffer is full, %d draw calls dropped", this->dropped_draw_calls_count);
    }

    light_map_sync_darkness_size(this, native_renderer);

    SDL_Texture* original_target = SDL_GetRenderTarget(native_renderer);

    SDL_SetTextureBlendMode(this->darkness_mask, SDL_BLENDMODE_NONE);
    SDL_SetRenderTarget(native_renderer, this->darkness_mask);

    SDL_SetRenderDrawColor(native_renderer, this->darkness_color.r, this->darkness_color.g, this->darkness_color.b, 255);
    SDL_RenderClear(native_renderer);

    SDL_BlendMode original_blend_mode;
    SDL_GetRenderDrawBlendMode(native_renderer, &original_blend_mode);

    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct light_map_draw_call draw_call = this->draw_calls[i];

        if (!renderer_primitive_is_visible(draw_call.primitive, draw_call.rect, viewport)) {
            continue;
        }

        SDL_SetRenderDrawBlendMode(native_renderer, this->light_source_blend_mode);
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, viewport, output_size, native_renderer);
    }
    light_map_clear_draw_calls(this);

    SDL_SetRenderDrawBlendMode(native_renderer, original_blend_mode);
    SDL_SetRenderTarget(native_renderer, original_target);

    SDL_SetTextureBlendMode(this->darkness_mask, SDL_BLENDMODE_MOD);
    SDL_RenderTexture(native_renderer, this->darkness_mask, NULL, NULL);
}

static bool light_map_is_visible(struct renderer_primitive* base, struct rect rect, struct rect viewport) {
    return true;
}

void light_map_reset(struct light_map* this) {
    light_map_reset_darkness_color(this);
    light_map_reset_enable(this);
}

void light_map_set_darkness_color(struct light_map* this, const struct color color) {
    this->darkness_color = color;
}
void light_map_reset_darkness_color(struct light_map* this) {
    this->darkness_color = (struct color){
        .r = 50,
        .g = 50,
        .b = 80,
        .a = 255
    };
}

void light_map_set_enable(struct light_map* this, const bool value) {
    if (this->enabled && !value && this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
        this->darkness_mask = NULL;
    }
    this->enabled = value;
}
void light_map_reset_enable(struct light_map* this) {
    light_map_set_enable(this, true);
}
