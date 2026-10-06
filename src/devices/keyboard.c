//
// Created by nikita on 24.09.2026.
//

#include "keyboard.h"

#include <SDL3/SDL.h>

#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include "../logging/logger.h"
#include "../msystems/msystem.h"

#define SDL3_SCANCODE_NUMBER 512


static struct {
    bool status[SDL3_SCANCODE_NUMBER];

    struct action on_key_pressed[SDL3_SCANCODE_NUMBER];
    struct action on_key_released[SDL3_SCANCODE_NUMBER];

    struct action* on_native_event;
    unsigned int subscription_token;
} keyboard;


static void keyboard_on_create(void) {
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        keyboard.on_key_pressed[i] = action_create();
        keyboard.on_key_released[i] = action_create();
    }
}
static void keyboard_on_destroy(void) {
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        action_destroy(&keyboard.on_key_pressed[i]);
        action_destroy(&keyboard.on_key_released[i]);
    }
}

static void keyboard_release_all(void) {
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        if (keyboard.status[i]) {
            keyboard.status[i] = false;
            action_invoke(&keyboard.on_key_released[i], NULL);
        }
    }
}

static void keyboard_register_native_event(void* listener, void* context) {
    const SDL_Event *event = context;
    const int scancode = event->key.scancode;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.repeat) {
            return;
        }
        logger_debug("Keyboard registered key down");
        keyboard.status[scancode] = true;
        action_invoke(&keyboard.on_key_pressed[scancode], NULL);
    }
    else if (event->type == SDL_EVENT_KEY_UP) {
        logger_debug("Keyboard registered key up");
        keyboard.status[scancode] = false;
        action_invoke(&keyboard.on_key_released[scancode], NULL);
    }
    else if (event->type == SDL_EVENT_WINDOW_FOCUS_LOST) {
        // The key up events of a window without the keyboard focus go to somebody else
        logger_debug("Keyboard lost focus");
        keyboard_release_all();
    }
}
static void keyboard_on_enable(struct engine_arguments args) {
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        keyboard.status[i] = 0;
    }
    keyboard.on_native_event = engine_events_on_native_event();
    action_subscribe(keyboard.on_native_event, NULL, keyboard_register_native_event, &keyboard.subscription_token);
}
static void keyboard_on_disable(void) {
    action_unsubscribe(keyboard.on_native_event, keyboard.subscription_token);
    keyboard.on_native_event = NULL;
    keyboard.subscription_token = 0;
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        action_clear(&keyboard.on_key_pressed[i]);
        action_clear(&keyboard.on_key_released[i]);
    }
}

const struct msystem keyboard_msystem = {
    .name = "keyboard",
    .on_create = keyboard_on_create,
    .on_enable = keyboard_on_enable,
    .on_disable = keyboard_on_disable,
    .on_destroy = keyboard_on_destroy
};

static int keyboard_keycode_to_native_keycode(const char* keycode) {
    const SDL_Scancode scancode = SDL_GetScancodeFromName(keycode);
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        logger_warn("Keyboard does not know key %s", keycode);
    }
    return scancode;
}

bool keyboard_is_pressed(const char* keycode) {
    return keyboard.status[keyboard_keycode_to_native_keycode(keycode)];
}

struct action* keyboard_require_action_on_key_pressed(const char* keycode) {
    return &keyboard.on_key_pressed[keyboard_keycode_to_native_keycode(keycode)];
}
struct action* keyboard_require_action_on_key_released(const char* keycode) {
    return &keyboard.on_key_released[keyboard_keycode_to_native_keycode(keycode)];
}
