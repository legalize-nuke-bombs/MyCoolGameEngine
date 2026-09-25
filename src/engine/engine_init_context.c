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


struct engine_init_context {
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct renderer_pipeline *renderer_pipeline;

    struct component_fabric *component_fabric;
    struct interpreter *interpreter;
};


struct engine_init_context* engine_init_context_try_create(struct engine *engine, struct engine_init_arguments arguments) {
    logger_init(arguments.logger_level);
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    logger_info("Engine init context is creating...");
    struct engine_init_context *this = malloc(sizeof(struct engine_init_context));

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        logger_error("SDL_Init failed: %s", SDL_GetError());
        return NULL;
    }
    this->window = NULL;
    this->renderer = NULL;
    if (SDL_CreateWindowAndRenderer("MyCoolGameEngine", 800, 600, SDL_WINDOW_RESIZABLE, &this->window, &this->renderer)) {
        SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
    }
    else {
        logger_error("Engine init context failed to create window: %s", SDL_GetError());
        free(this);
        return NULL;
    }
    this->renderer_pipeline = renderer_pipeline_create(this->renderer);

    this->component_fabric = component_fabric_create();
    this->interpreter = interpreter_create(engine);

    return this;
}
void engine_init_context_destroy(struct engine_init_context *this) {
    logger_info("Engine init context is destroying...");

    interpreter_destroy(this->interpreter);
    component_fabric_destroy(this->component_fabric);

    renderer_pipeline_destroy(this->renderer_pipeline);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    free(this);
}


struct renderer_pipeline* engine_init_context_get_renderer_pipeline(const struct engine_init_context *this) {
    return this->renderer_pipeline;
}
struct interpreter* engine_init_context_get_interpreter(const struct engine_init_context *this) {
    return this->interpreter;
}
struct component_fabric* engine_init_context_get_component_fabric(const struct engine_init_context *this) {
    return this->component_fabric;
}