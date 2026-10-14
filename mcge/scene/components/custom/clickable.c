//
// Created by Nikita on 14.10.2026.
//

#include "clickable.h"

#include "mcge/devices/mouse.h"
#include "mcge/logging/logger.h"
#include "mcge/scene/components/component_internal.h"
#include "mcge/utils/action.h"


struct clickable {
    struct component component;

    struct action on_pressed;
    struct action on_released;

    unsigned int on_mouse_button_pressed_token;
    unsigned int on_mouse_button_released_token;
};


const char* clickable_component_key(void) {
    return "clickable";
}


static void clickable_on_create(struct component* base, struct fields *fields) {
    struct clickable* this = (struct clickable*)base;
    this->on_pressed = action_create();
    this->on_released = action_create();
}
static void clickable_on_destroy(struct component* base) {
    struct clickable* this = (struct clickable*)base;
    action_destroy(&this->on_pressed);
    action_destroy(&this->on_released);
}

static void handle_mouse_button_pressed(void *listener, void *context);
static void handle_mouse_button_released(void *listener, void *context);

static void clickable_awake(struct component* base) {
    struct clickable* this = (struct clickable*)base;
    action_subscribe(mouse_get_on_button_pressed(), this, handle_mouse_button_pressed, &this->on_mouse_button_pressed_token);
    action_subscribe(mouse_get_on_button_released(), this, handle_mouse_button_released, &this->on_mouse_button_released_token);
}
static void clickable_on_disable(struct component* base) {
    const struct clickable* this = (struct clickable*)base;
    action_unsubscribe(mouse_get_on_button_pressed(), this->on_mouse_button_pressed_token);
    action_unsubscribe(mouse_get_on_button_released(), this->on_mouse_button_released_token);
}


const struct component_vtable clickable_vtable = {
    .component_key = clickable_component_key,
    .size = sizeof(struct clickable),
    .on_create = clickable_on_create,
    .on_destroy = clickable_on_destroy,
    .on_awake = clickable_awake,
    .on_disable = clickable_on_disable
};


static void handle_mouse_button_pressed(void *listener, void *context) {
    struct clickable* this = listener;
    struct mouse_on_button_callback_context *cnt = context;

    const struct rect rect = component_get_rect((struct component*)this);
    const struct vector2 point = cnt->position;
    if (!rect_contains(rect, point)) {
        return;
    }
    logger_debug("Clickable mouse button pressed");
    action_invoke(&this->on_pressed, &cnt);
}
static void handle_mouse_button_released(void *listener, void *context) {
    struct clickable* this = listener;
    struct mouse_on_button_callback_context *cnt = context;

    const struct rect rect = component_get_rect((struct component*)this);
    const struct vector2 point = cnt->position;
    if (!rect_contains(rect, point)) {
        return;
    }
    logger_debug("Clickable mouse button released");
    action_invoke(&this->on_released, &cnt);
}