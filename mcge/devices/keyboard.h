//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_KEYBOARD_H
#define MYCOOLGAMEENGINE_KEYBOARD_H
#include <stdbool.h>
#include "../api.h"

struct msystem;
struct action;

MCGE_API extern const struct msystem keyboard_msystem;

MCGE_API bool keyboard_is_pressed(const char* keycode);

MCGE_API struct action* keyboard_require_action_on_key_pressed(const char* keycode);
MCGE_API struct action* keyboard_require_action_on_key_released(const char* keycode);

#endif //MYCOOLGAMEENGINE_KEYBOARD_H
