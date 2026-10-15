//
// Created by nikita on 16.10.2026.
//

#include "fullscreen_toggler.h"

#include "../component_internal.h"
#include "mcge/allocator/allocator.h"
#include "mcge/devices/keyboard.h"
#include "mcge/rendering/renderer.h"
#include "mcge/utils/action.h"
#include "mcge/utils/fields.h"


struct fullscreen_toggler {
    struct component base;

    char *hotkey;

    struct action* on_hotkey_pressed;
    unsigned int on_hotkey_pressed_token;
};


const char* fullscreen_toggler_component_key(void) {
    return "fullscreen_toggler";
}


static void fullscreen_toggler_on_create(struct component* base, struct fields *fields) {
    struct fullscreen_toggler* this = (struct fullscreen_toggler*)base;
    this->hotkey = fields_dup_string(fields, "key", "F11");
}
static void fullscreen_toggler_on_destroy(struct component* base) {
    const struct fullscreen_toggler* this = (struct fullscreen_toggler*)base;
    allocator_delete(this->hotkey);
}


static void handle_hotkey_pressed(void *listener, void *context);

static void fullscreen_toggler_awake(struct component* base) {
    struct fullscreen_toggler* this = (struct fullscreen_toggler*)base;
    this->on_hotkey_pressed = keyboard_require_action_on_key_pressed(this->hotkey);
    action_subscribe(this->on_hotkey_pressed, this, handle_hotkey_pressed, &this->on_hotkey_pressed_token);
}
static void fullscreen_toggler_on_disable(struct component* base) {
    const struct fullscreen_toggler* this = (struct fullscreen_toggler*)base;
    action_unsubscribe(this->on_hotkey_pressed, this->on_hotkey_pressed_token);
}


const struct component_vtable fullscreen_toggler_vtable = {
    .component_key = fullscreen_toggler_component_key,
    .size = sizeof(struct fullscreen_toggler),
    .on_create = fullscreen_toggler_on_create,
    .on_destroy = fullscreen_toggler_on_destroy,
    .on_awake = fullscreen_toggler_awake,
    .on_disable = fullscreen_toggler_on_disable
};


static void handle_hotkey_pressed(void *listener, void *context) {
    renderer_set_fullscreen(!renderer_is_fullscreen());
}
