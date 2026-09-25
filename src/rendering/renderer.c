//
// Created by Nikita on 25.09.2026.
//

#include "renderer.h"

#include <stdlib.h>

#include "../engine/engine.h"
#include "../logging/logger.h"
#include "../engine/engine_execution_context.h"
#include <SDL3/SDL.h>

#include "../engine/engine_init_context.h"


struct renderer {
    SDL_Renderer *native;
    struct renderer_pipeline *pipeline;
    struct engine *engine;
};


struct renderer* renderer_create(struct engine* engine) {
    logger_info("Renderer is creating...");
    struct renderer* this = malloc(sizeof(struct renderer));
    this->native = NULL;
    this->pipeline = NULL;
    this->engine = engine;
    return this;
}
void renderer_destroy(struct renderer *this) {
    logger_info("Renderer is destroying...");
    free(this);
}

void renderer_awake(struct renderer *this) {
    logger_info("Renderer is awaking...");
    struct engine_init_context* engine_init_context = engine_get_init_context(this->engine);
    this->native = engine_init_context_get_native_renderer(engine_init_context);
    this->pipeline = engine_init_context_get_renderer_pipeline(engine_init_context);
}
void renderer_disable(struct renderer *this) {
    logger_info("Renderer is disabling...");
}