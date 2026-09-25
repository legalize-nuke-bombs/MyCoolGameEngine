//
// Created by Nikita on 25.09.2026.
//

#include "renderer.h"

#include <stdlib.h>

#include "../engine/engine.h"
#include "../logging/logger.h"
#include "../engine/events/engine_events.h"
#include "../engine/engine_init_context.h"
#include "../utils/action.h"
#include <SDL3/SDL.h>

#include "renderer_pipeline.h"


struct renderer {
    SDL_Renderer *native;
    struct renderer_pipeline *pipeline;

    struct action* on_rendering;
    unsigned int on_rendering_subscription_token;
    struct engine *engine;
};


struct renderer* renderer_create(struct engine* engine) {
    logger_info("Renderer is creating...");
    struct renderer* this = malloc(sizeof(struct renderer));

    this->native = NULL;
    this->pipeline = NULL;

    this->on_rendering = NULL;
    this->on_rendering_subscription_token = 0;
    this->engine = engine;

    return this;
}
void renderer_destroy(struct renderer *this) {
    logger_info("Renderer is destroying...");
    free(this);
}

static void renderer_render(void *listener, void *context) {
    const struct renderer* this = listener;
    SDL_SetRenderDrawColor(this->native, 0, 0, 0, 255);
    SDL_RenderClear(this->native);
    renderer_pipeline_flush(this->pipeline);
    SDL_RenderPresent(this->native);
}

void renderer_awake(struct renderer *this) {
    logger_info("Renderer is awaking...");
    const struct engine_init_context* engine_init_context = engine_get_init_context(this->engine);
    this->native = engine_init_context_get_native_renderer(engine_init_context);
    this->pipeline = engine_init_context_get_renderer_pipeline(engine_init_context);
    this->on_rendering = engine_events_on_rendering(engine_init_context_get_events(engine_get_init_context(this->engine)));
    action_subscribe(this->on_rendering, this, renderer_render, &this->on_rendering_subscription_token);
}
void renderer_disable(struct renderer *this) {
    logger_info("Renderer is disabling...");
    this->native = NULL;
    this->pipeline = NULL;
    action_unsubscribe(this->on_rendering, this->on_rendering_subscription_token);
    this->on_rendering = NULL;
    this->on_rendering_subscription_token = 0;
}