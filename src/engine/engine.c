//
// Created by nikita on 24.09.2026.
//

#include "engine.h"

#include <stdlib.h>

#include "../logging/logger.h"
#include <SDL3/SDL.h>
#include "engine_execution_context.h"
#include "engine_init_context.h"
#include "../interpreter/interpreter.h"
#include "update_context.h"
#include "../utils/action.h"
#include "engine_events.h"

struct engine {
    struct engine_init_context *init_context;
    struct engine_events *events;
    struct engine_execution_context *execution_context;
};

struct engine* engine_try_create(struct engine_init_arguments arguments) {
    logger_info("Engine is creating...");
    struct engine *this = malloc(sizeof(struct engine));
    this->init_context = engine_init_context_try_create(this, arguments);
    if (this->init_context == NULL) {
        free(this);
        return NULL;
    }
    return this;
}
void engine_destroy(struct engine *this) {
    logger_info("Engine is destroying...");
    engine_init_context_destroy(this->init_context);
    free(this);
}

void engine_execute(struct engine *this, const struct engine_execution_arguments arguments) {
    if (arguments.script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
    }
    logger_info("Engine is executing script %s...", arguments.script_path);

    this->events = engine_events_create();
    this->execution_context = engine_execution_context_create(this, arguments);

    const struct interpreter* interpreter = engine_init_context_get_interpreter(this->init_context);
    const int code = interpreter_eval(interpreter, arguments.script_path);
    if (code == INTERPRETER_OK) {
        logger_info("Interpreter finished with exit code %d", code);
    }
    else {
        engine_execution_context_destroy(this->execution_context);
        logger_error("Interpreter finished with exit code %d", code);
        return;
    }

    engine_init_context_awake(this->init_context);
    engine_execution_context_awake(this->execution_context);

    struct update_context update_context = {
        .dt = 0
    };
    Uint64 previous = SDL_GetTicksNS();

    while (engine_execution_context_is_running(this->execution_context)) {
        const Uint64 now = SDL_GetTicksNS();
        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;

        action_invoke(engine_events_pre_frame(this->events), &update_context);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            action_invoke(engine_events_on_native_event(this->events), &event);
        }

        action_invoke(engine_events_pre_physics(this->events), &update_context);
        action_invoke(engine_events_on_physics(this->events), &update_context);
        action_invoke(engine_events_post_physics(this->events), &update_context);

        action_invoke(engine_events_pre_rendering(this->events), &update_context);
        action_invoke(engine_events_on_rendering(this->events), &update_context);
        action_invoke(engine_events_post_rendering(this->events), &update_context);
    }

    engine_init_context_disable(this->init_context);
    engine_execution_context_destroy(this->execution_context);
    engine_events_destroy(this->events);
}

struct engine_init_context* engine_get_init_context(const struct engine *this) {
    return this->init_context;
}
struct engine_events* engine_get_events(const struct engine *this) {
    return this->events;
}
struct engine_execution_context* engine_get_execution_context(const struct engine *this) {
    return this->execution_context;
}