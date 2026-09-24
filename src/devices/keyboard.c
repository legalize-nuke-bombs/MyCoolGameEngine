//
// Created by nikita on 24.09.2026.
//

#include "keyboard.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../utils/action.h"
#include "../logging/logger.h"

#define SDL3_SCANCODE_NUMBER 512


struct keyboard {
    struct action* on_key_pressed[SDL3_SCANCODE_NUMBER];
    struct action* on_key_released[SDL3_SCANCODE_NUMBER];
};

struct keyboard* keyboard_create() {
    logger_info("Keyboard is creating...");
    struct keyboard* this = calloc(1, sizeof(struct keyboard));
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

struct action* keyboard_try_get_action_on_key_pressed(const struct keyboard *this, const char* keycode) {
    return this->on_key_pressed[keyboard_keycode_to_native_keycode(keycode)];
}
struct action* keyboard_try_get_action_on_key_released(const struct keyboard *this, const char* keycode) {
    return this->on_key_released[keyboard_keycode_to_native_keycode(keycode)];
}

struct action* keyboard_require_action_on_key_pressed(struct keyboard *this, const char* keycode) {
    return keyboard_require_action(&this->on_key_pressed[keyboard_keycode_to_native_keycode(keycode)]);
}
struct action* keyboard_require_action_on_key_released(struct keyboard *this, const char* keycode) {
    return keyboard_require_action(&this->on_key_released[keyboard_keycode_to_native_keycode(keycode)]);
}
