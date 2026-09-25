//
// Created by Nikita on 25.09.2026.
//

#include "engine_closer.h"
#include "../../../logging/logger.h"
#include "../../engine.h"
#include <stdlib.h>

#include "../../events/engine_events.h"
#include "../../../utils/action.h"
#include <SDL3/SDL.h>
#include "../../engine_execution_context.h"
#include "../../engine_init_context.h"

struct engine_closer {
    struct engine *engine;
    struct action* on_native_event;
    unsigned int on_native_event_subscription_token;
};

struct engine_closer* engine_closer_create(struct engine *engine) {
    logger_info("Engine closer is creating...");
    struct engine_closer* this = malloc(sizeof(struct engine_closer));
    this->engine = engine;
    this->on_native_event = NULL;
    this->on_native_event_subscription_token = 0;
    return this;
}
void engine_closer_destroy(struct engine_closer *this) {
    logger_info("Engine closer is destroying...");
    free(this);
}

static void engine_closer_handle_native_event(void* listener, void* context) {
    const struct engine_closer* this = listener;
    const SDL_Event *event = context;
    if (event->type == SDL_EVENT_QUIT) {
        logger_info("Engine closer fired");
        engine_execution_context_mark_stop_required(engine_get_execution_context(this->engine));
    }
}

void engine_closer_awake(struct engine_closer *this) {
    logger_info("Engine closer is awaking...");
    this->on_native_event = engine_events_on_native_event(engine_init_context_get_events(engine_get_init_context(this->engine)));
    action_subscribe(this->on_native_event, this, engine_closer_handle_native_event, &this->on_native_event_subscription_token);
}
void engine_closer_disable(struct engine_closer *this) {
    logger_info("Engine closer is disabling...");
    action_unsubscribe(this->on_native_event, this->on_native_event_subscription_token);
    this->on_native_event = NULL;
    this->on_native_event_subscription_token = 0;
}