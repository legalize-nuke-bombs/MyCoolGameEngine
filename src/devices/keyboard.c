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

    bool status[SDL3_SCANCODE_NUMBER];

    struct action on_key_pressed[SDL3_SCANCODE_NUMBER];
    struct action on_key_released[SDL3_SCANCODE_NUMBER];

    struct action* on_native_event;
    unsigned int subscription_token;
};

static const char* keyboard_get_name() {
    return "keyboard";
}
static void keyboard_on_destroy(struct subsystem* base);
static void keyboard_on_enable(struct subsystem* base, struct engine_arguments args);
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
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        this->on_key_pressed[i] = action_create();
        this->on_key_released[i] = action_create();
    }
    return base;
}
void keyboard_on_destroy(struct subsystem *base) {
    struct keyboard* this = (struct keyboard*)base;
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        action_destroy(&this->on_key_pressed[i]);
        action_destroy(&this->on_key_released[i]);
    }
}

static void keyboard_register_native_event(void* listener, void* context) {
    struct keyboard *this = listener;
    const SDL_Event *event = context;
    const int scancode = event->key.scancode;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.repeat) {
            return;
        }
        logger_debug("Keyboard registered key down");
        this->status[scancode] = true;
        action_invoke(&this->on_key_pressed[scancode], NULL);
    }
    else if (event->type == SDL_EVENT_KEY_UP) {
        logger_debug("Keyboard registered key up");
        this->status[scancode] = false;
        action_invoke(&this->on_key_released[scancode], NULL);
    }
}
void keyboard_on_enable(struct subsystem* base, struct engine_arguments args) {
    struct keyboard* this = (struct keyboard*)base;
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        this->status[i] = 0;
    }
    this->on_native_event = engine_events_on_native_event((struct engine_events*)subsystem_get_subsystem(base, "engine_events"));
    action_subscribe(this->on_native_event, this, keyboard_register_native_event, &this->subscription_token);
}
void keyboard_on_disable(struct subsystem* base) {
    struct keyboard* this = (struct keyboard*)base;
    action_unsubscribe(this->on_native_event, this->subscription_token);
    this->on_native_event = NULL;
    this->subscription_token = 0;
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        action_clear(&this->on_key_pressed[i]);
        action_clear(&this->on_key_released[i]);
    }
}

static int keyboard_keycode_to_native_keycode(const char* keycode) {
    const SDL_Scancode scancode = SDL_GetScancodeFromName(keycode);
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        logger_warn("Keyboard does not know key %s", keycode);
    }
    return scancode;
}

bool keyboard_is_pressed(const struct keyboard *this, const char* keycode) {
    return this->status[keyboard_keycode_to_native_keycode(keycode)];
}

struct action* keyboard_require_action_on_key_pressed(struct keyboard *this, const char* keycode) {
    return &this->on_key_pressed[keyboard_keycode_to_native_keycode(keycode)];
}
struct action* keyboard_require_action_on_key_released(struct keyboard *this, const char* keycode) {
    return &this->on_key_released[keyboard_keycode_to_native_keycode(keycode)];
}
