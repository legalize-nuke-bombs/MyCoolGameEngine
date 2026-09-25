//
// Created by nikita on 24.09.2026.
//

#include "keyboard.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../engine/engine.h"
#include "../engine/engine_execution_context.h"
#include "../engine/engine_events.h"
#include "../utils/action.h"
#include "../logging/logger.h"

#define SDL3_SCANCODE_NUMBER 512


struct keyboard {
    struct action* on_key_pressed[SDL3_SCANCODE_NUMBER];
    struct action* on_key_released[SDL3_SCANCODE_NUMBER];

    struct action* on_native_event;
    unsigned int subscription_token;

    struct engine* engine;
};

struct keyboard* keyboard_create(struct engine *engine) {
    logger_info("Keyboard is creating...");
    struct keyboard* this = calloc(1, sizeof(struct keyboard));
    this->engine = engine;
    return this;
}
void keyboard_destroy(struct keyboard *this) {
    logger_info("Keyboard is destroying...");
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        if (this->on_key_pressed[i] != NULL) {
            action_destroy(this->on_key_pressed[i]);
        }
        if (this->on_key_released[i] != NULL) {
            action_destroy(this->on_key_released[i]);
        }
    }
    free(this);
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
void keyboard_awake(struct keyboard *this) {
    logger_info("Keyboard is awaking...");
    this->on_native_event = engine_events_on_native_event(engine_get_events(this->engine));
    action_subscribe(this->on_native_event, this, keyboard_register_native_event, &this->subscription_token);
}
void keyboard_disable(struct keyboard *this) {
    logger_info("Keyboard is disabling...");
    action_unsubscribe(this->on_native_event, this->subscription_token);
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
