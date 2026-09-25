//
// Created by Nikita on 25.09.2026.
//

#include "renderer.h"

#include <stdlib.h>

#include "../engine/engine.h"
#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include <SDL3/SDL.h>
#include "../logging/logger.h"
#include "renderer_layer_manager.h"
#include "../subsystems/subsystem_internal.h"
#include "renderer_pipeline.h"


struct renderer {
    struct subsystem base;

    SDL_Window *window;
    SDL_Renderer *native;
    struct renderer_layer_manager *layer_manager;
    struct renderer_pipeline *pipeline;

    struct action* on_rendering;
    unsigned int on_rendering_subscription_token;
};


static const char* renderer_get_name() {
    return "renderer";
}
static void renderer_on_destroy(struct subsystem* base);
static void renderer_on_enable(struct subsystem* base);
static void renderer_on_disable(struct subsystem* base);

static struct subsystem_vtable renderer_vtable = {
    .name = renderer_get_name,
    .on_destroy = renderer_on_destroy,
    .on_enable = renderer_on_enable,
    .on_disable = renderer_on_disable
};


struct subsystem* renderer_create(const struct subsystem_collection* subsystems) {
    struct renderer* this = calloc(1, sizeof(struct renderer));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &renderer_vtable, subsystems);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        logger_error("SDL_Init failed: %s", SDL_GetError());
        return NULL;
    }
    if (SDL_CreateWindowAndRenderer("MyCoolGameEngine", 800, 600, SDL_WINDOW_RESIZABLE, &this->window, &this->native)) {
        SDL_SetRenderDrawBlendMode(this->native, SDL_BLENDMODE_BLEND);
    }
    else {
        logger_error("SDL Failed to create window and renderer: %s", SDL_GetError());
        return NULL;
    }
    this->layer_manager = renderer_layer_manager_create();
    this->pipeline = renderer_pipeline_create(this->native);

    return base;
}
void renderer_on_destroy(struct subsystem* base) {
    const struct renderer* this = (struct renderer*)base;

    renderer_pipeline_destroy(this->pipeline);
    renderer_layer_manager_destroy(this->layer_manager);
    SDL_DestroyRenderer(this->native);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    free(base);
}

static void renderer_render(void *listener, void *context) {
    const struct renderer* this = listener;
    SDL_SetRenderDrawColor(this->native, 0, 0, 0, 255);
    SDL_RenderClear(this->native);
    renderer_pipeline_flush(this->pipeline);
    SDL_RenderPresent(this->native);
}

void renderer_on_enable(struct subsystem* base) {
    struct renderer* this = (struct renderer*)base;
    this->on_rendering = engine_events_on_rendering((struct engine_events*)subsystem_get_subsystem(base, "engine_events"));
    action_subscribe(this->on_rendering, this, renderer_render, &this->on_rendering_subscription_token);
}
void renderer_on_disable(struct subsystem* base) {
    struct renderer* this = (struct renderer*)base;
    action_unsubscribe(this->on_rendering, this->on_rendering_subscription_token);
    this->on_rendering = NULL;
    renderer_layer_manager_clear(this->layer_manager);
}

struct renderer_pipeline* renderer_get_pipeline(const struct renderer* this) {
    return this->pipeline;
}
struct renderer_layer_manager* renderer_get_layer_manager(const struct renderer* this) {
    return this->layer_manager;
}