//
// Created by nikita on 24.09.2026.
//

#include "engine.h"

#include <stdbool.h>

#include "../logging/logger.h"
#include <SDL3/SDL.h>
#include "../interpreter/interpreter.h"
#include "update_context.h"
#include "../utils/action.h"
#include "events/engine_events.h"
#include "version.h"
#include "../msystems/msystem.h"
#include "lifecycle/engine_lifecycle.h"
#include "utils/engine_closer.h"
#include "utils/engine_restarter.h"
#include "../assets/assets.h"
#include "../devices/keyboard.h"
#include "../modules/bt/bt_module.h"
#include "../profiler/profiler.h"
#include "../random/random.h"
#include "../rendering/renderer.h"
#include "../scene/scene.h"

#define FRAME_DT_EXPLOSION_THRESHOLD 0.1f
#define ENGINE_MSYSTEMS_MAX 32

static struct {
    const struct msystem *msystems[ENGINE_MSYSTEMS_MAX];
    bool enabled[ENGINE_MSYSTEMS_MAX];
    int msystems_count;
} engine;

void engine_register_msystem(const struct msystem *msystem) {
    if (engine.msystems_count >= ENGINE_MSYSTEMS_MAX) {
        logger_error("Engine failed to register msystem `%s`: there are %d msystems already", msystem->name, engine.msystems_count);
        return;
    }
    logger_info("Msystem `%s` is creating...", msystem->name);
    if (msystem->on_create) {
        msystem->on_create();
    }
    engine.enabled[engine.msystems_count] = false;
    engine.msystems[engine.msystems_count] = msystem;
    engine.msystems_count++;
}

static void engine_enable_all(const struct engine_arguments args) {
    logger_info("Engine is enabling all msystems...");
    for (int i = 0; i < engine.msystems_count; i++) {
        if (engine.enabled[i]) {
            continue;
        }
        logger_debug("Msystem `%s` is enabling...", engine.msystems[i]->name);
        engine.enabled[i] = true;
        if (engine.msystems[i]->on_enable) {
            engine.msystems[i]->on_enable(args);
        }
    }
}
static void engine_disable_all(void) {
    logger_info("Engine is disabling all msystems...");
    for (int i = engine.msystems_count - 1; i >= 0; i--) {
        if (!engine.enabled[i]) {
            continue;
        }
        logger_debug("Msystem `%s` is disabling...", engine.msystems[i]->name);
        engine.enabled[i] = false;
        if (engine.msystems[i]->on_disable) {
            engine.msystems[i]->on_disable();
        }
    }
}

void engine_create(void) {
    logger_info("MyCoolGameEngine v%d.%d.%d", ENGINE_V_MAJOR, ENGINE_V_MINOR, ENGINE_V_PATCH);
    logger_info("Engine is creating...");
    engine_register_msystem(&engine_lifecycle_msystem);
    engine_register_msystem(&engine_events_msystem);
    engine_register_msystem(&keyboard_msystem);
    engine_register_msystem(&assets_msystem);
    engine_register_msystem(&bt_msystem);
    engine_register_msystem(&scene_msystem);
    engine_register_msystem(&renderer_msystem);
    engine_register_msystem(&interpreter_msystem);
    engine_register_msystem(&profiler_msystem);
    engine_register_msystem(&engine_closer_msystem);
    engine_register_msystem(&engine_restarter_msystem);
    engine_register_msystem(&random_msystem);
    logger_info("Engine knows %d msystems", engine.msystems_count);
}
void engine_destroy(void) {
    logger_info("Engine is destroying...");
    engine_disable_all();
    for (int i = engine.msystems_count - 1; i >= 0; i--) {
        logger_info("Msystem `%s` is destroying...", engine.msystems[i]->name);
        if (engine.msystems[i]->on_destroy) {
            engine.msystems[i]->on_destroy();
        }
    }
    engine.msystems_count = 0;
}

static bool engine_execute_step(const struct engine_arguments args) {
    if (args.script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
        return 0;
    }
    engine_enable_all(args);

    if (interpreter_eval(args.script_path) != INTERPRETER_OK) {
        engine_disable_all();
        return 0;
    }

    struct update_context update_context = {
        .dt = 0,
        .frame_number = 0
    };
    Uint64 previous = SDL_GetTicksNS();

    while (engine_lifecycle_is_running()) {
        const Uint64 now = SDL_GetTicksNS();
        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;
        if (update_context.dt > FRAME_DT_EXPLOSION_THRESHOLD) {
            logger_debug("Frame dt explosion resolved (%f -> %f)", update_context.dt, FRAME_DT_EXPLOSION_THRESHOLD);
            update_context.dt = FRAME_DT_EXPLOSION_THRESHOLD;
        }
        update_context.frame_number++;

        action_invoke(engine_events_pre_frame(), &update_context);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            action_invoke(engine_events_on_native_event(), &event);
        }

        action_invoke(engine_events_pre_physics(), &update_context);
        action_invoke(engine_events_on_physics(), &update_context);
        action_invoke(engine_events_post_physics(), &update_context);

        action_invoke(engine_events_pre_rendering(), &update_context);
        action_invoke(engine_events_on_rendering(), &update_context);
        action_invoke(engine_events_post_rendering(), &update_context);
    }

    engine_disable_all();

    return engine_lifecycle_restart_required();
}

void engine_execute(const struct engine_arguments args) {
    while (engine_execute_step(args)) {}
}
