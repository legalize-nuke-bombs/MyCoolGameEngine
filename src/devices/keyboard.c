//
// Created by nikita on 24.09.2026.
//

#include "keyboard.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include "../logging/logger.h"
#include "../subsystems/subsystem_internal.h"

#define SDL3_SCANCODE_NUMBER 512


struct keyboard {
    struct subsystem base;

    struct action* on_key_pressed[SDL3_SCANCODE_NUMBER];
    struct action* on_key_released[SDL3_SCANCODE_NUMBER];

    struct action* on_native_event;
    unsigned int subscription_token;
};

static const char* keyboard_get_name() {
    return "keyboard";
}
static void keyboard_on_destroy(struct subsystem* base);
static void keyboard_on_enable(struct subsystem* base);
static void keyboard_on_disable(struct subsystem* base);

static struct subsystem_vtable keyboard_vtable = {
    .name = keyboard_get_name,
    .on_destroy = keyboard_on_destroy,
    .on_enable = keyboard_on_enable,
    .on_disable = keyboard_on_disable
};

struct subsystem* keyboard_create(const struct subsystem_collection* subsystems) {
    struct keyboard* this = calloc(1, sizeof(struct keyboard));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &keyboard_vtable, subsystems);
    return base;
}
void keyboard_on_destroy(struct subsystem *base) {
    const struct keyboard* this = (struct keyboard*)base;
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        if (this->on_key_pressed[i] != NULL) {
            action_destroy(this->on_key_pressed[i]);
        }
        if (this->on_key_released[i] != NULL) {
            action_destroy(this->on_key_released[i]);
        }
    }
}

static void keyboard_register_native_event(void* listener, void* context) {
    const struct keyboard *this = listener;
    const SDL_Event *event = context;
    const int scancode = event->key.scancode;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        logger_debug("Keyboard registered key down");
        const struct action* on_key_pressed = this->on_key_pressed[scancode];
        if (on_key_pressed != NULL) {
            action_invoke(on_key_pressed, NULL);
        }
    }
    else if (event->type == SDL_EVENT_KEY_UP) {
        logger_debug("Keyboard registered key up");
        const struct action* on_key_released = this->on_key_released[scancode];
        if (on_key_released != NULL) {
            action_invoke(on_key_released, NULL);
        }
    }
}
void keyboard_on_enable(struct subsystem* base) {
    struct keyboard* this = (struct keyboard*)base;
    this->on_native_event = engine_events_on_native_event((struct engine_events*)subsystem_get_subsystem(base, "engine_events"));
    action_subscribe(this->on_native_event, this, keyboard_register_native_event, &this->subscription_token);
}
void keyboard_on_disable(struct subsystem* base) {
    struct keyboard* this = (struct keyboard*)base;
    action_unsubscribe(this->on_native_event, this->subscription_token);
    this->on_native_event = NULL;
    this->subscription_token = 0;
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        if (this->on_key_pressed[i] != NULL) {
            action_clear(this->on_key_pressed[i]);
        }
        if (this->on_key_released[i] != NULL) {
            action_clear(this->on_key_released[i]);
        }
    }
}

static int keyboard_keycode_to_native_keycode(const char* keycode) {
    const SDL_Scancode scancode = SDL_GetScancodeFromName(keycode);
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        logger_warn("Keyboard does not know key %s", keycode);
    }
    return scancode;
}

static struct action* keyboard_require_action(struct action **slot) {
    if (*slot == NULL) {
        *slot = action_create();
    }
    return *slot;
}
struct action* keyboard_require_action_on_key_pressed(struct keyboard *this, const char* keycode) {
    return keyboard_require_action(&this->on_key_pressed[keyboard_keycode_to_native_keycode(keycode)]);
}
struct action* keyboard_require_action_on_key_released(struct keyboard *this, const char* keycode) {
    return keyboard_require_action(&this->on_key_released[keyboard_keycode_to_native_keycode(keycode)]);
}
