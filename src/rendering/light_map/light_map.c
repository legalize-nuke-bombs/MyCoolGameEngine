//
// Created by nikita on 26.09.2026.
//

#include "light_map.h"
#include <SDL3/SDL.h>

#include "../renderer_pipeline.h"
#include "../../utils/action.h"
#include "../../logging/logger.h"


#define DARKNESS_R 20
#define DARKNESS_G 20
#define DARKNESS_B 20
#define DARKNESS_A 100

#define DRAW_CALLS_BUFFER_SIZE (1 << 10)


struct light_map {
    SDL_Texture *darkness_mask;

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
}

static void light_map_resize(void* listener, void* context) {
    logger_info("Renderer lightning map is resizing...");
    struct light_map* this = listener;
    const struct vector2* size = context;
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
    }
    this->darkness_mask = SDL_CreateTexture(this->native_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size->x, (int)size->y);
}

static void light_map_flush(void* listener, void* context) {
    struct light_map* this = listener;
    const struct rect* viewport = context;

    SDL_SetRenderTarget(this->native_renderer, this->darkness_mask);
    SDL_SetRenderDrawColor(this->native_renderer, DARKNESS_R, DARKNESS_G, DARKNESS_B, DARKNESS_A);
    SDL_RenderClear(this->native_renderer);

    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct light_map_draw_call draw_call = this->draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, *viewport, this->native_renderer);
    }
    this->draw_calls_count = 0;

    SDL_SetRenderTarget(this->native_renderer, NULL);
    SDL_RenderTexture(this->native_renderer, this->darkness_mask, NULL, NULL);
}