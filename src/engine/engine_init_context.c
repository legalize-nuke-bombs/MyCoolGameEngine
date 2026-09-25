//
// Created by Nikita on 25.09.2026.
//

#include "engine_init_context.h"

#include <stdlib.h>
#include "../logging/logger.h"
#include <SDL3/SDL.h>

#include "../interpreter/interpreter.h"
#include "../rendering/renderer_pipeline.h"
#include "../scene/components/component_fabric.h"
#include "version.h"
#include "../devices/devices.h"
#include "../rendering/renderer.h"
#include "events/engine_events.h"
#include "utils/engine_utils.h"


struct engine_init_context {
    struct engine_events* events;
    struct devices *devices;

    SDL_Window *window;
    SDL_Renderer *native_renderer;
    struct renderer_pipeline *renderer_pipeline;
    struct renderer *renderer;

    struct component_fabric *component_fabric;
    struct interpreter *interpreter;
    struct engine_utils *utils;
};


struct engine_init_context* engine_init_context_try_create(struct engine *engine, struct engine_init_arguments arguments) {
    logger_init(arguments.logger_level);
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    logger_info("Engine init context is creating...");
    struct engine_init_context *this = malloc(sizeof(struct engine_init_context));

    this->events = engine_events_create();
    this->devices = devices_create(engine);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        logger_error("SDL_Init failed: %s", SDL_GetError());
        return NULL;
    }
    this->window = NULL;
    this->native_renderer = NULL;
    if (SDL_CreateWindowAndRenderer("MyCoolGameEngine", 800, 600, SDL_WINDOW_RESIZABLE, &this->window, &this->native_renderer)) {
        SDL_SetRenderDrawBlendMode(this->native_renderer, SDL_BLENDMODE_BLEND);
    }
    else {
        logger_error("Engine init context failed to create window: %s", SDL_GetError());
        devices_destroy(this->devices);
        engine_events_destroy(this->events);
        free(this);
        return NULL;
    }
    this->renderer_pipeline = renderer_pipeline_create(this->native_renderer);
    this->renderer = renderer_create(engine);

    this->component_fabric = component_fabric_create();
    this->interpreter = interpreter_create(engine);
    this->utils = engine_utils_create(engine, arguments);

    return this;
}
void engine_init_context_destroy(struct engine_init_context *this) {
    logger_info("Engine init context is destroying...");

    engine_utils_destroy(this->utils);
    interpreter_destroy(this->interpreter);
    component_fabric_destroy(this->component_fabric);

    renderer_destroy(this->renderer);
    renderer_pipeline_destroy(this->renderer_pipeline);
    SDL_DestroyRenderer(this->native_renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    devices_destroy(this->devices);
    engine_events_destroy(this->events);

    free(this);
}


void engine_init_context_awake(const struct engine_init_context *this) {
    logger_info("Engine init context is awaking...");
    renderer_awake(this->renderer);
    devices_awake(this->devices);
    engine_utils_awake(this->utils);
    engine_events_awake(this->events);
}
void engine_init_context_disable(const struct engine_init_context *this) {
    logger_info("Engine init context is disabling...");
    renderer_disable(this->renderer);
    engine_utils_disable(this->utils);
    devices_disable(this->devices);
    engine_events_disable(this->events);
}


struct engine_events* engine_init_context_get_events(const struct engine_init_context *this) {
    return this->events;
}
SDL_Renderer* engine_init_context_get_native_renderer(const struct engine_init_context *this) {
    return this->native_renderer;
}
struct renderer_pipeline* engine_init_context_get_renderer_pipeline(const struct engine_init_context *this) {
    return this->renderer_pipeline;
}
struct devices* engine_init_context_get_devices(struct engine_init_context *this) {
    return this->devices;
}
struct interpreter* engine_init_context_get_interpreter(const struct engine_init_context *this) {
    return this->interpreter;
}
struct component_fabric* engine_init_context_get_component_fabric(const struct engine_init_context *this) {
    return this->component_fabric;
}