//
// Created by Nikita on 25.09.2026.
//

#include "engine_closer.h"
#include "../../logging/logger.h"
#include <stdlib.h>

#include "../events/engine_events.h"
#include "../../utils/action.h"
#include <SDL3/SDL.h>
#include "../../subsystems/subsystem_internal.h"

struct engine_closer {
    struct subsystem base;

    struct action* on_native_event;
    unsigned int on_native_event_subscription_token;
};

static const char* engine_closer_get_name() {
    return "engine_closer";
}

static void engine_closer_on_enable(struct subsystem* base);
static void engine_closer_on_disable(struct subsystem* base);

static struct subsystem_vtable engine_closer_vtable = {
    .name = engine_closer_get_name,
    .on_destroy = NULL,
    .on_enable = engine_closer_on_enable,
    .on_disable = engine_closer_on_disable
};

struct subsystem* engine_closer_create(struct subsystem_collection *subsystems) {
    struct engine_closer* this = calloc(1, sizeof(struct engine_closer));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &engine_closer_vtable, subsystems);
    return base;
}

static void engine_closer_handle_native_event(void* listener, void* context) {
    const struct engine_closer* this = listener;
    const SDL_Event *event = context;
    if (event->type == SDL_EVENT_QUIT) {
        logger_info("Engine closer fired");
        // TODO
    }
}

void engine_closer_on_enable(struct subsystem *base) {
    struct engine_closer* this = (struct engine_closer*)base;
    this->on_native_event = engine_events_on_native_event((struct engine_events*)subsystem_get_subsystem(base, "engine_events"));
    action_subscribe(this->on_native_event, this, engine_closer_handle_native_event, &this->on_native_event_subscription_token);
}
void engine_closer_on_disable(struct subsystem *base) {
    struct engine_closer* this = (struct engine_closer*)base;
    action_unsubscribe(this->on_native_event, this->on_native_event_subscription_token);
    this->on_native_event = NULL;
    this->on_native_event_subscription_token = 0;
}