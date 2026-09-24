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
    struct keyboard* this = malloc(sizeof(struct keyboard));
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        this->on_key_pressed[i] = NULL;
    }
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        this->on_key_released[i] = NULL;
    }
    return this;
}
void keyboard_destroy(struct keyboard *this) {
    logger_info("Keyboard is destroying...");
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        struct action* action = this->on_key_pressed[i];
        if (action == NULL) {
            continue;
        }
        action_destroy(action);
    }
    for (int i = 0; i < SDL3_SCANCODE_NUMBER; i++) {
        struct action* action = this->on_key_released[i];
        if (action == NULL) {
            continue;
        }
        action_destroy(this->on_key_released[i]);
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

struct action* keyboard_get_action_on_key_pressed(struct keyboard *this, const char* keycode) {
    const int native_keycode = keyboard_keycode_to_native_keycode(keycode);
    if (this->on_key_pressed[native_keycode]) {
        return this->on_key_pressed[native_keycode];
    }
    this->on_key_pressed[native_keycode] = action_create();
    return this->on_key_pressed[native_keycode];
}
struct action* keyboard_get_action_on_key_released(struct keyboard *this, const char* keycode) {
    const int native_keycode = keyboard_keycode_to_native_keycode(keycode);
    if (this->on_key_released[native_keycode]) {
        return this->on_key_released[native_keycode];
    }
    this->on_key_released[native_keycode] = action_create();
    return this->on_key_released[native_keycode];
}