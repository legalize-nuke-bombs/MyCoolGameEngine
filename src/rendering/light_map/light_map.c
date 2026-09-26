//
// Created by nikita on 26.09.2026.
//

#include "light_map.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../renderer_pipeline.h"
#include "../../utils/action.h"
#include "../../logging/logger.h"
#include "../../utils/color.h"

#define DRAW_CALLS_BUFFER_SIZE (1 << 10)


struct light_map {
    struct color darkness_color;
    SDL_Texture *darkness_mask;
    SDL_BlendMode light_source_blend_mode;

    SDL_Renderer* native_renderer;

    struct light_map_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;

    struct action* on_viewpoint_resize;
    unsigned int on_viewpoint_resize_token;

    struct action* on_post_process;
    unsigned int on_post_process_token;
};

static void light_map_resize(void* listener, void* context);
static void light_map_flush(void* listener, void* context);


struct light_map* light_map_create(struct renderer_pipeline *pipeline) {
    logger_info("Renderer lightning map is creating...");
    struct light_map* this = calloc(1, sizeof(struct light_map));

    light_map_reset(this);
    this->light_source_blend_mode = SDL_ComposeCustomBlendMode(
       SDL_BLENDFACTOR_ZERO,
       SDL_BLENDFACTOR_ONE,
       SDL_BLENDOPERATION_ADD,
       SDL_BLENDFACTOR_ZERO,
       SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
       SDL_BLENDOPERATION_ADD
   );

    this->native_renderer = renderer_pipeline_get_native_renderer(pipeline);

    this->on_viewpoint_resize = renderer_pipeline_on_viewpoint_resize(pipeline);
    action_subscribe(this->on_viewpoint_resize, this, light_map_resize, &this->on_viewpoint_resize_token);

    this->on_post_process = renderer_pipeline_on_post_process(pipeline);
    action_subscribe(this->on_post_process, this, light_map_flush, &this->on_post_process_token);
    return this;
}
void light_map_destroy(struct light_map* this) {
    logger_info("Renderer lightning map is destroying...");
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
        this->darkness_mask = NULL;
    }
    action_unsubscribe(this->on_viewpoint_resize, this->on_viewpoint_resize_token);
    action_unsubscribe(this->on_post_process, this->on_post_process_token);
    free(this);
}

static void light_map_resize(void* listener, void* context) {
    logger_info("Renderer lightning map is resizing...");
    struct light_map* this = listener;
    const struct vector2* size = context;
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
    }
    this->darkness_mask = SDL_CreateTexture(this->native_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size->x, (int)size->y);
    SDL_SetTextureBlendMode(this->darkness_mask, SDL_BLENDMODE_BLEND);
}

void light_map_draw_primitive(struct light_map* this, const struct light_map_draw_call draw_call) {
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE) {
        logger_warn("Light map buffer size overflow");
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

static void light_map_flush(void* listener, void* context) {
    struct light_map* this = listener;
    const struct rect* viewport = context;

    SDL_Texture* original_target = SDL_GetRenderTarget(this->native_renderer);
    SDL_SetRenderTarget(this->native_renderer, this->darkness_mask);

    SDL_SetRenderDrawColor(this->native_renderer, this->darkness_color.r, this->darkness_color.g, this->darkness_color.b, this->darkness_color.a);
    SDL_RenderClear(this->native_renderer);

    SDL_BlendMode original_blend_mode;
    SDL_GetRenderDrawBlendMode(this->native_renderer, &original_blend_mode);
    SDL_SetRenderDrawBlendMode(this->native_renderer, this->light_source_blend_mode);

    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct light_map_draw_call draw_call = this->draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, *viewport, this->native_renderer);
    }
    this->draw_calls_count = 0;

    SDL_SetRenderDrawBlendMode(this->native_renderer, original_blend_mode);

    SDL_SetRenderTarget(this->native_renderer, original_target);

    SDL_RenderTexture(this->native_renderer, this->darkness_mask, NULL, NULL);
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