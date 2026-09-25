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
#include "events/engine_events.h"
#include "version.h"
#include "../subsystems/subsystem_collection.h"

struct engine {
    struct subsystem_collection *subsystems;
    struct engine_init_context *init_context;
    struct engine_execution_context *execution_context;
};

struct engine* engine_create(struct engine_init_arguments arguments) {
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    logger_info("Engine is creating...");
    struct engine *this = malloc(sizeof(struct engine));
    this->subsystems = subsystem_collection_create();
    this->init_context = engine_init_context_try_create(this, arguments);
    if (this->init_context == NULL) {
        free(this);
        return NULL;
    }
    return this;
}
void engine_destroy(struct engine *this) {
    logger_info("Engine is destroying...");
    subsystem_collection_destroy(this->subsystems);
    engine_init_context_destroy(this->init_context);
    free(this);
}

static bool engine_execute_step(struct engine *this, const struct engine_execution_arguments arguments) {
    if (arguments.script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
        return 0;
    }
    this->execution_context = engine_execution_context_create(this, arguments);

    const struct interpreter* interpreter = engine_init_context_get_interpreter(this->init_context);
    if (interpreter_eval(interpreter, arguments.script_path) != INTERPRETER_OK) {
        engine_execution_context_destroy(this->execution_context);
        return 0;
    }

    engine_init_context_awake(this->init_context);
    engine_execution_context_awake(this->execution_context);

    struct update_context update_context = {
        .dt = 0
    };
    Uint64 previous = SDL_GetTicksNS();

    struct engine_events* events = engine_init_context_get_events(this->init_context);
    while (engine_execution_context_is_running(this->execution_context)) {
        const Uint64 now = SDL_GetTicksNS();
        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;

        action_invoke(engine_events_pre_frame(events), &update_context);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            action_invoke(engine_events_on_native_event(events), &event);
        }

        action_invoke(engine_events_pre_physics(events), &update_context);
        action_invoke(engine_events_on_physics(events), &update_context);
        action_invoke(engine_events_post_physics(events), &update_context);

        action_invoke(engine_events_pre_rendering(events), &update_context);
        action_invoke(engine_events_on_rendering(events), &update_context);
        action_invoke(engine_events_post_rendering(events), &update_context);
    }

    const bool rerun_required = engine_execution_context_if_rerun_required(this->execution_context);

    engine_execution_context_disable(this->execution_context);
    engine_execution_context_destroy(this->execution_context);
    engine_init_context_disable(this->init_context);

    return rerun_required;
}

void engine_execute(struct engine *this, const struct engine_execution_arguments arguments) {
    while (engine_execute_step(this, arguments)) {}
}

struct engine_init_context* engine_get_init_context(const struct engine *this) {
    return this->init_context;
}
struct engine_execution_context* engine_get_execution_context(const struct engine *this) {
    return this->execution_context;
}
struct subsystem_collection* engine_get_subsystems(const struct engine* this) {
    return this->subsystems;
}