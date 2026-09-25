//
// Created by Nikita on 25.09.2026.
//

#include <stdlib.h>
#include <string.h>

#include "engine_execution_context.h"
#include "../profiler/profiler.h"
#include "../rendering/renderer_layer_manager.h"
#include "../scene/scene.h"
#include "../logging/logger.h"
#include "engine.h"
#include "../subsystems/subsystem.h"


struct engine_execution_context {
    bool running;
    bool rerun_required;

    struct engine_execution_arguments arguments;

    struct renderer_layer_manager *renderer_layer_manager;
    struct scene *scene;
    struct profiler *profiler;
};


struct engine_execution_context* engine_execution_context_create(struct engine *engine, struct engine_execution_arguments arguments) {
    logger_info("Engine execution context is creating...");
    struct engine_execution_context *this = malloc(sizeof(struct engine_execution_context));

    this->running = false;
    this->rerun_required = false;
    this->arguments = arguments;
    this->renderer_layer_manager = renderer_layer_manager_create();
    this->scene = (struct scene*)scene_create(strdup("Default scene"), engine, engine_get_subsystems(engine));
    this->profiler = profiler_create(engine);

    return this;
}
void engine_execution_context_destroy(struct engine_execution_context *this) {
    logger_info("Engine execution context is destroying...");

    profiler_destroy(this->profiler);
    subsystem_destroy((struct subsystem*)this->scene);
    renderer_layer_manager_destroy(this->renderer_layer_manager);

    free(this);
}


void engine_execution_context_awake(struct engine_execution_context *this) {
    logger_info("Engine execution context is awaking...");

    this->running = true;
    subsystem_enable((struct subsystem*)this->scene);
    profiler_awake(this->profiler);
}
void engine_execution_context_disable(struct engine_execution_context *this) {
    logger_info("Engine execution context is disabling...");

    subsystem_disable((struct subsystem*)this->scene);
    profiler_disable(this->profiler);
}



bool engine_execution_context_is_running(const struct engine_execution_context *this) {
    return this->running;
}
void engine_execution_context_mark_stop_required(struct engine_execution_context *this) {
    this->running = false;
}

bool engine_execution_context_if_rerun_required(const struct engine_execution_context *this) {
    return this->rerun_required;
}
void engine_execution_context_mark_rerun_required(struct engine_execution_context *this) {
    this->rerun_required = true;
}

struct engine_execution_arguments engine_execution_context_get_arguments(struct engine_execution_context *this) {
    return this->arguments;
}

struct renderer_layer_manager* engine_execution_context_get_renderer_layer_manager(const struct engine_execution_context *this) {
    return this->renderer_layer_manager;
}
struct scene* engine_execution_context_get_scene(const struct engine_execution_context *this) {
    return this->scene;
}