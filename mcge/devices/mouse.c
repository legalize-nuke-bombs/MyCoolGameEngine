//
// Created by nikita on 13.10.2026.
//

#include "mouse.h"
#include "../engine/msystem.h"
#include "mcge/engine/events/engine_events.h"
#include "mcge/utils/action.h"
#include <SDL3/SDL.h>


static struct {
    struct action on_wheel_scrolled;

    struct action* on_native_event;
    unsigned int on_native_event_token;
} mouse;



static void mouse_on_create(void) {
    mouse.on_wheel_scrolled = action_create();
}
static void mouse_on_destroy(void) {
    action_destroy(&mouse.on_wheel_scrolled);
}


static void handle_native_event(void *listener, void *context);

static void mouse_on_enable(struct engine_arguments args) {
    mouse.on_native_event = engine_events_on_native_event();
    action_subscribe(mouse.on_native_event, NULL, handle_native_event, &mouse.on_native_event_token);
}
static void mouse_on_disable(void) {
    action_unsubscribe(mouse.on_native_event, mouse.on_native_event_token);
}


const struct msystem mouse_msystem = {
    .name = "mouse",
    .on_create = mouse_on_create,
    .on_destroy = mouse_on_destroy,
    .on_enable = mouse_on_enable,
    .on_disable = mouse_on_disable
};




static void handle_native_event(void *listener, void *context) {
    const SDL_Event *event = context;
    if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        struct mouse_on_wheel_scrolled_callback_context cnt = {
            .dx = -event->wheel.x,
            .dy = -event->wheel.y
        };
        action_invoke(&mouse.on_wheel_scrolled, &cnt);
    }
}



struct action* mouse_get_on_wheel_scrolled() {
    return &mouse.on_wheel_scrolled;
}