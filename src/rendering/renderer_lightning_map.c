//
// Created by nikita on 26.09.2026.
//

#include "renderer_lightning_map.h"
#include <SDL3/SDL.h>

#include "renderer_pipeline.h"
#include "../utils/action.h"
#include "../logging/logger.h"


struct renderer_lightning_map {
    SDL_Texture *darkness_mask;

    SDL_Renderer* native_renderer;

    struct action* on_viewpoint_resize;
    unsigned int on_viewpoint_resize_token;

    struct action* on_post_process;
    unsigned int on_post_process_token;
};

static void renderer_lightning_map_resize(void* listener, void* context);
static void renderer_lightning_map_draw(void* listener, void* context);


struct renderer_lightning_map* renderer_lightning_map_create(struct renderer_pipeline *pipeline) {
    logger_info("Renderer lightning map is creating...");
    struct renderer_lightning_map* this = calloc(1, sizeof(struct renderer_lightning_map));
    this->native_renderer = renderer_pipeline_get_native_renderer(pipeline);

    this->on_viewpoint_resize = renderer_pipeline_on_viewpoint_resize(pipeline);
    action_subscribe(this->on_viewpoint_resize, this, renderer_lightning_map_resize, &this->on_viewpoint_resize_token);

    this->on_post_process = renderer_pipeline_on_post_process(pipeline);
    action_subscribe(this->on_post_process, this, renderer_lightning_map_draw, &this->on_post_process_token);
    return this;
}
void renderer_lightning_map_destroy(struct renderer_lightning_map* this) {
    logger_info("Renderer lightning map is destroying...");
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
        this->darkness_mask = NULL;
    }
    action_unsubscribe(this->on_viewpoint_resize, this->on_viewpoint_resize_token);
    action_unsubscribe(this->on_post_process, this->on_post_process_token);
}

static void renderer_lightning_map_resize(void* listener, void* context) {
    logger_info("Renderer lightning map is resizing...");
    struct renderer_lightning_map* this = listener;
    const struct vector2* size = context;
    if (this->darkness_mask != NULL) {
        SDL_DestroyTexture(this->darkness_mask);
    }
    this->darkness_mask = SDL_CreateTexture(this->native_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size->x, (int)size->y);
}

static void renderer_lightning_map_draw(void* listener, void* context) {

}