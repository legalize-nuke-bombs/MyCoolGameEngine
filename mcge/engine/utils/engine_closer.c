//
// Created by Nikita on 25.09.2026.
//

#include "engine_closer.h"
#include "../../logging/logger.h"

#include "../events/engine_events.h"
#include "../../utils/action.h"
#include <SDL3/SDL.h>
#include "../../msystems/msystem.h"
#include "../lifecycle/engine_lifecycle.h"

static struct {
    struct action* on_native_event;
    unsigned int on_native_event_subscription_token;
} engine_closer;

static void engine_closer_handle_native_event(void* listener, void* context) {
    const SDL_Event *event = context;
    if (event->type == SDL_EVENT_QUIT) {
        logger_info("Engine closer fired");
        engine_lifecycle_mark_stop();
    }
}

static void engine_closer_on_enable(struct engine_arguments args) {
    engine_closer.on_native_event = engine_events_on_native_event();
    action_subscribe(engine_closer.on_native_event, NULL, engine_closer_handle_native_event, &engine_closer.on_native_event_subscription_token);
}
static void engine_closer_on_disable(void) {
    action_unsubscribe(engine_closer.on_native_event, engine_closer.on_native_event_subscription_token);
    engine_closer.on_native_event = NULL;
}

const struct msystem engine_closer_msystem = {
    .name = "engine_closer",
    .on_enable = engine_closer_on_enable,
    .on_disable = engine_closer_on_disable
};
