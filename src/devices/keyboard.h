//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_KEYBOARD_H
#define MYCOOLGAMEENGINE_KEYBOARD_H
#include <stdbool.h>

struct msystem;
struct action;

extern const struct msystem keyboard_msystem;

bool keyboard_is_pressed(const char* keycode);

struct action* keyboard_require_action_on_key_pressed(const char* keycode);
struct action* keyboard_require_action_on_key_released(const char* keycode);

#endif //MYCOOLGAMEENGINE_KEYBOARD_H
