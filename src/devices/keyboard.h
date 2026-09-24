//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_KEYBOARD_H
#define MYCOOLGAMEENGINE_KEYBOARD_H

struct keyboard;
struct action;

struct keyboard* keyboard_create();
void keyboard_destroy(struct keyboard *this);

struct action* keyboard_try_get_action_on_key_pressed(const struct keyboard *this, const char* keycode);
struct action* keyboard_try_get_action_on_key_released(const struct keyboard *this, const char* keycode);

struct action* keyboard_require_action_on_key_pressed(struct keyboard *this, const char* keycode);
struct action* keyboard_require_action_on_key_released(struct keyboard *this, const char* keycode);

#endif //MYCOOLGAMEENGINE_KEYBOARD_H
