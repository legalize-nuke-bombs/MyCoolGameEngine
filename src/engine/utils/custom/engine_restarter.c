//
// Created by Nikita on 25.09.2026.
//

#include "engine_restarter.h"

#include <stdlib.h>

#include "../../engine.h"
#include "../../engine_execution_context.h"
#include "../../../logging/logger.h"
#include "../../engine_init_context.h"
#include "../../../devices/devices.h"
#include "../../../devices/keyboard.h"
#include "../../../utils/action.h"
#include "../../../utils/file_listener.h"
#include "../../events/engine_events.h"
#include "../../update_context.h"


struct engine_restarter {
    struct file_listener *script_listener;

    unsigned int on_hotkey_token;
    struct action* on_hotkey;

    unsigned int pre_frame_token;
    struct action* pre_frame;

    struct engine *engine;
};


struct engine_restarter* engine_restarter_create(struct engine *engine, struct engine_init_arguments arguments) {
    logger_info("Engine restarter is creating...");
    struct engine_restarter* this = malloc(sizeof(struct engine_restarter));
    this->script_listener = NULL;
    this->on_hotkey = NULL;
    this->pre_frame = NULL;
    if (arguments.dev_mode) {
        this->engine = engine;
    }
    else {
        this->engine = NULL;
    }
    return this;
}
void engine_restarter_destroy(struct engine_restarter *this) {
    logger_info("Engine restarter is destroying...");
    free(this);
}

static void engine_restarter_fire(struct engine *engine) {
    logger_info("Engine restarter fired");
    struct engine_execution_context* engine_execution_context = engine_get_execution_context(engine);
    engine_execution_context_mark_rerun_required(engine_execution_context);
    engine_execution_context_mark_stop_required(engine_execution_context);
}


static void engine_restarter_handle_hotkey(void* listener, void *context) {
    const struct engine_restarter *this = listener;
    engine_restarter_fire(this->engine);
}

static void engine_restarter_handle_pre_frame(void *listener, void *context) {
    const struct engine_restarter *this = listener;
    const struct update_context* update_context = context;
    if (file_listener_update(this->script_listener, update_context->dt)) {
        engine_restarter_fire(this->engine);
    }
}

void engine_restarter_awake(struct engine_restarter *this) {
    if (this->engine == NULL) {
        return;
    }
    logger_info("Engine restarter is awaking...");

    this->script_listener = file_listener_create(engine_execution_context_get_arguments(engine_get_execution_context(this->engine)).script_path);

    this->on_hotkey = keyboard_require_action_on_key_pressed(devices_get_keyboard(engine_init_context_get_devices(engine_get_init_context(this->engine))), "F5");
    action_subscribe(this->on_hotkey, this, engine_restarter_handle_hotkey, &this->on_hotkey_token);

    this->pre_frame = engine_events_pre_frame(engine_init_context_get_events(engine_get_init_context(this->engine)));
    action_subscribe(this->pre_frame, this, engine_restarter_handle_pre_frame, &this->pre_frame_token);
}
void engine_restarter_disable(struct engine_restarter *this) {
    if (this->engine == NULL) {
        return;
    }
    logger_info("Engine restarter is disabling...");

    file_listener_destroy(this->script_listener);

    action_unsubscribe(this->on_hotkey, this->on_hotkey_token);
    this->on_hotkey = NULL;

    action_unsubscribe(this->pre_frame, this->pre_frame_token);
    this->pre_frame = NULL;
}