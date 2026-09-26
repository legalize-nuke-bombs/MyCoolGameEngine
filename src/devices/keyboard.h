//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_KEYBOARD_H
#define MYCOOLGAMEENGINE_KEYBOARD_H
#include <stdbool.h>

struct subsystem_collection;
struct keyboard;
struct action;

struct subsystem* keyboard_create(const struct subsystem_collection *subsystems);

bool keyboard_is_pressed(const struct keyboard *this, const char* keycode);

struct action* keyboard_require_action_on_key_pressed(struct keyboard *this, const char* keycode);
struct action* keyboard_require_action_on_key_released(struct keyboard *this, const char* keycode);

#endif //MYCOOLGAMEENGINE_KEYBOARD_H
