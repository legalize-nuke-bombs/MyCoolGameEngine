//
// Created by Nikita on 25.09.2026.
//

#include "engine_restarter.h"

#include <stdlib.h>

#include "../../logging/logger.h"
#include "../../devices/keyboard.h"
#include "../../utils/action.h"
#include "../../utils/file_listener.h"
#include "../events/engine_events.h"
#include "../update_context.h"
#include "../../subsystems/subsystem_internal.h"


struct engine_restarter {
    struct subsystem base;

    struct file_listener *script_listener;

    unsigned int on_hotkey_token;
    struct action* on_hotkey;

    unsigned int pre_frame_token;
    struct action* pre_frame;
};


static const char* engine_restarter_get_name() {
    return "engine_restarter";
}

static void engine_restarter_on_enable(struct subsystem* base);
static void engine_restarter_on_disable(struct subsystem* base);

static struct subsystem_vtable engine_restarter_vtable = {
    .name = engine_restarter_get_name,
    .on_destroy = NULL,
    .on_enable = engine_restarter_on_enable,
    .on_disable = engine_restarter_on_disable
};


struct subsystem* engine_restarter_create(struct subsystem_collection *subsystems) {
    struct engine_restarter* this = calloc(1, sizeof(struct engine_restarter));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &engine_restarter_vtable, subsystems);
    return base;
}

static void engine_restarter_fire(struct subsystem* this) {
    logger_info("Engine restarter fired");
    // TODO
}


static void engine_restarter_handle_hotkey(void* listener, void *context) {
    struct subsystem *this = listener;
    engine_restarter_fire(this);
}

static void engine_restarter_handle_pre_frame(void *listener, void *context) {
    const struct engine_restarter *this = listener;
    const struct update_context* update_context = context;
    if (file_listener_update(this->script_listener, update_context->dt)) {
        engine_restarter_fire((struct subsystem*)this);
    }
}

void engine_restarter_on_enable(struct subsystem *base) {
    struct engine_restarter* this = (struct engine_restarter*)base;
    this->script_listener = file_listener_create("123");

    this->on_hotkey = keyboard_require_action_on_key_pressed((struct keyboard*)subsystem_get_subsystem(base, "keyboard"), "F5");
    action_subscribe(this->on_hotkey, this, engine_restarter_handle_hotkey, &this->on_hotkey_token);

    this->pre_frame = engine_events_pre_frame((struct engine_events*)subsystem_get_subsystem(base, "engine_events"));
    action_subscribe(this->pre_frame, this, engine_restarter_handle_pre_frame, &this->pre_frame_token);
}
void engine_restarter_on_disable(struct subsystem *base) {
    struct engine_restarter* this = (struct engine_restarter*)base;
    file_listener_destroy(this->script_listener);

    action_unsubscribe(this->on_hotkey, this->on_hotkey_token);
    this->on_hotkey = NULL;

    action_unsubscribe(this->pre_frame, this->pre_frame_token);
    this->pre_frame = NULL;
}