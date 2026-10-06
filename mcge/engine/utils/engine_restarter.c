//
// Created by Nikita on 25.09.2026.
//

#include "engine_restarter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../../logging/logger.h"
#include "../../devices/keyboard.h"
#include "../../interpreter/interpreter.h"
#include "../../utils/action.h"
#include "../../utils/file_listener.h"
#include "../../utils/list.h"
#include "../events/engine_events.h"
#include "../update_context.h"
#include "../../msystems/msystem.h"
#include "../lifecycle/engine_lifecycle.h"


static struct {
    struct list script_listeners;

    unsigned int on_hotkey_token;
    struct action* on_hotkey;

    unsigned int pre_frame_token;
    struct action* pre_frame;

    unsigned int on_script_evaluated_token;
    struct action* on_script_evaluated;
} engine_restarter;


static void engine_restarter_on_create(void) {
    engine_restarter.script_listeners = list_create(4);
}
static void engine_restarter_on_destroy(void) {
    list_destroy(&engine_restarter.script_listeners);
}

static void engine_restarter_fire(void) {
    logger_info("Engine restarter fired");
    engine_lifecycle_mark_restart();
    engine_lifecycle_mark_stop();
}


static void engine_restarter_handle_hotkey(void* listener, void *context) {
    engine_restarter_fire();
}

static void engine_restarter_handle_pre_frame(void *listener, void *context) {
    const struct update_context* update_context = context;
    bool changed = false;
    for (int i = 0; i < list_count(&engine_restarter.script_listeners); i++) {
        if (file_listener_update(list_get(&engine_restarter.script_listeners, i), update_context->dt)) {
            changed = true;
        }
    }
    if (changed) {
        engine_restarter_fire();
    }
}

static void engine_restarter_handle_script_evaluated(void *listener, void *context) {
    const char* script_path = context;
    for (int i = 0; i < list_count(&engine_restarter.script_listeners); i++) {
        if (strcmp(file_listener_get_path(list_get(&engine_restarter.script_listeners, i)), script_path) == 0) {
            return;
        }
    }
    list_add(&engine_restarter.script_listeners, file_listener_create(script_path));
}

static void engine_restarter_on_enable(const struct engine_arguments args) {
    if (!args.dev_mode) {
        return;
    }

    engine_restarter.on_hotkey = keyboard_require_action_on_key_pressed("F5");
    action_subscribe(engine_restarter.on_hotkey, NULL, engine_restarter_handle_hotkey, &engine_restarter.on_hotkey_token);

    engine_restarter.pre_frame = engine_events_pre_frame();
    action_subscribe(engine_restarter.pre_frame, NULL, engine_restarter_handle_pre_frame, &engine_restarter.pre_frame_token);

    engine_restarter.on_script_evaluated = interpreter_get_action_on_script_evaluated();
    action_subscribe(engine_restarter.on_script_evaluated, NULL, engine_restarter_handle_script_evaluated, &engine_restarter.on_script_evaluated_token);
}
static void engine_restarter_on_disable(void) {
    for (int i = 0; i < list_count(&engine_restarter.script_listeners); i++) {
        file_listener_destroy(list_get(&engine_restarter.script_listeners, i));
    }
    list_clear(&engine_restarter.script_listeners);

    if (engine_restarter.on_hotkey != NULL) {
        action_unsubscribe(engine_restarter.on_hotkey, engine_restarter.on_hotkey_token);
        engine_restarter.on_hotkey = NULL;
    }

    if (engine_restarter.pre_frame != NULL) {
        action_unsubscribe(engine_restarter.pre_frame, engine_restarter.pre_frame_token);
        engine_restarter.pre_frame = NULL;
    }

    if (engine_restarter.on_script_evaluated != NULL) {
        action_unsubscribe(engine_restarter.on_script_evaluated, engine_restarter.on_script_evaluated_token);
        engine_restarter.on_script_evaluated = NULL;
    }
}

const struct msystem engine_restarter_msystem = {
    .name = "engine_restarter",
    .on_create = engine_restarter_on_create,
    .on_enable = engine_restarter_on_enable,
    .on_disable = engine_restarter_on_disable,
    .on_destroy = engine_restarter_on_destroy
};
