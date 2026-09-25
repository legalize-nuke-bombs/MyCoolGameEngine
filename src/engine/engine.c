//
// Created by nikita on 24.09.2026.
//

#include "engine.h"

#include <stdlib.h>

#include "../logging/logger.h"
#include <SDL3/SDL.h>
#include "../interpreter/interpreter.h"
#include "update_context.h"
#include "../utils/action.h"
#include "events/engine_events.h"
#include "version.h"
#include "../subsystems/subsystem_collection.h"
#include "lifecycle/engine_lifecycle.h"

struct engine {
    struct subsystem_collection *subsystems;
};

struct engine* engine_create() {
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    logger_info("Engine is creating...");
    struct engine *this = malloc(sizeof(struct engine));
    this->subsystems = subsystem_collection_create();
    return this;
}
void engine_destroy(struct engine *this) {
    logger_info("Engine is destroying...");
    subsystem_collection_destroy(this->subsystems);
    free(this);
}

static bool engine_execute_step(const struct engine *this, const struct engine_arguments args) {
    if (args.script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
        return 0;
    }
    subsystem_collection_enable_all(this->subsystems, args);

    struct interpreter* interpreter = (struct interpreter*)subsystem_collection_get(this->subsystems, "interpreter");
    struct engine_lifecycle* lifecycle = (struct engine_lifecycle*)subsystem_collection_get(this->subsystems, "engine_lifecycle");
    struct engine_events* events = (struct engine_events*)subsystem_collection_get(this->subsystems, "engine_events");

    if (interpreter_eval(interpreter, args.script_path) != INTERPRETER_OK) {
        subsystem_collection_disable_all(this->subsystems);
        return 0;
    }

    struct update_context update_context = {
        .dt = 0
    };
    Uint64 previous = SDL_GetTicksNS();

    while (engine_lifecycle_is_running(lifecycle)) {
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

    subsystem_collection_disable_all(this->subsystems);

    return engine_lifecycle_restart_required(lifecycle);
}

void engine_execute(struct engine *this, const struct engine_arguments args) {
    while (engine_execute_step(this, args)) {}
}