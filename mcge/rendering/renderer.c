//
// Created by Nikita on 25.09.2026.
//

#include "renderer.h"

#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include <SDL3/SDL.h>
#include "../logging/logger.h"
#include "../msystems/msystem.h"
#include "renderer_pipeline.h"


static struct {
    SDL_Window *window;
    SDL_Renderer *native;
    struct renderer_pipeline *pipeline;

    struct action* on_rendering;
    unsigned int on_rendering_subscription_token;
} renderer;


static void renderer_on_create(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        logger_error("SDL_Init failed: %s", SDL_GetError());
        return;
    }
    if (SDL_CreateWindowAndRenderer("MyCoolGameEngine", 800, 600, SDL_WINDOW_RESIZABLE, &renderer.window, &renderer.native)) {
        SDL_SetRenderDrawBlendMode(renderer.native, SDL_BLENDMODE_BLEND);
    }
    else {
        logger_error("SDL Failed to create window and renderer: %s", SDL_GetError());
        return;
    }
    renderer.pipeline = renderer_pipeline_create(renderer.native);
}
static void renderer_on_destroy(void) {
    renderer_pipeline_destroy(renderer.pipeline);
    SDL_DestroyRenderer(renderer.native);
    SDL_DestroyWindow(renderer.window);
    SDL_Quit();
}

static void renderer_update(void *listener, void *context) {
    SDL_SetRenderDrawColor(renderer.native, 0, 0, 0, 255);
    SDL_RenderClear(renderer.native);
    renderer_pipeline_flush(renderer.pipeline);
    SDL_RenderPresent(renderer.native);
}

static void renderer_on_enable(struct engine_arguments args) {
    renderer_pipeline_enable(renderer.pipeline);
    if (args.program_name) SDL_SetWindowTitle(renderer.window, args.program_name);
    renderer.on_rendering = engine_events_on_rendering();
    action_subscribe(renderer.on_rendering, NULL, renderer_update, &renderer.on_rendering_subscription_token);
}
static void renderer_on_disable(void) {
    action_unsubscribe(renderer.on_rendering, renderer.on_rendering_subscription_token);
    renderer.on_rendering = NULL;
    renderer_pipeline_disable(renderer.pipeline);
}

const struct msystem renderer_msystem = {
    .name = "renderer",
    .on_create = renderer_on_create,
    .on_enable = renderer_on_enable,
    .on_disable = renderer_on_disable,
    .on_destroy = renderer_on_destroy
};

SDL_Renderer* renderer_get_native_renderer(void) {
    return renderer.native;
}
struct renderer_pipeline* renderer_get_pipeline(void) {
    return renderer.pipeline;
}
