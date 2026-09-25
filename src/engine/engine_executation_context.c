//
// Created by Nikita on 25.09.2026.
//

#include <stdlib.h>
#include <_string.h>

#include "engine_execution_context.h"
#include "../devices/devices.h"
#include "../profiler/profiler.h"
#include "../rendering/renderer_layer_manager.h"
#include "../scene/scene.h"
#include "../logging/logger.h"
#include "dev_mode/engine_restarter.h"
#include "events/engine_events.h"


struct engine_execution_context {
    bool running;
    struct engine_events *events;
    struct renderer_layer_manager *renderer_layer_manager;
    struct scene *scene;
    struct devices *devices;
    struct profiler *profiler;
    struct engine_restarter *restarter;
};


struct engine_execution_context* engine_execution_context_create(struct engine *engine, struct engine_execution_arguments arguments) {
    logger_info("Engine execution context is creating...");
    struct engine_execution_context *this = malloc(sizeof(struct engine_execution_context));

    this->running = false;
    this->events = engine_events_create();
    this->renderer_layer_manager = renderer_layer_manager_create();
    this->scene = scene_create(strdup("Default scene"), engine);
    this->devices = devices_create(engine);
    this->profiler = profiler_create();
    this->restarter = engine_restarter_create(engine, arguments);

    return this;
}
void engine_execution_context_destroy(struct engine_execution_context *this) {
    logger_info("Engine execution context is destroying...");

    engine_restarter_destroy(this->restarter);
    profiler_destroy(this->profiler);
    devices_destroy(this->devices);
    scene_destroy(this->scene);
    renderer_layer_manager_destroy(this->renderer_layer_manager);
    engine_events_destroy(this->events);

    free(this);
}


void engine_execution_context_awake(struct engine_execution_context *this) {
    logger_info("Engine execution context is awaking...");

    this->running = true;
    devices_awake(this->devices);
}



bool engine_execution_context_is_running(const struct engine_execution_context *this) {
    return this->running;
}
void engine_execution_context_stop(struct engine_execution_context *this) {
    this->running = false;
}

struct engine_events* engine_execution_context_get_events(const struct engine_execution_context *this) {
    return this->events;
}
struct renderer_layer_manager* engine_execution_context_get_renderer_layer_manager(const struct engine_execution_context *this) {
    return this->renderer_layer_manager;
}
struct scene* engine_execution_context_get_scene(const struct engine_execution_context *this) {
    return this->scene;
}