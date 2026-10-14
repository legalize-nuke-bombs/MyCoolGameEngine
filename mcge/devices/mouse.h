//
// Created by nikita on 13.10.2026.
//

#ifndef MYCOOLGAMEENGINE_MOUSE_H
#define MYCOOLGAMEENGINE_MOUSE_H

#include <stdbool.h>

#include "mcge/api.h"
#include "mcge/utils/vector2.h"


MCGE_API extern const struct msystem mouse_msystem;

struct mouse_on_wheel_scrolled_callback_context {
    float dx;
    float dy;
};
MCGE_API struct action* mouse_get_on_wheel_scrolled();

struct mouse_on_button_callback_context {
    int button;
    struct vector2 position;
};
MCGE_API struct action* mouse_get_on_button_pressed();
MCGE_API struct action* mouse_get_on_button_released();

MCGE_API struct vector2 mouse_get_position();

#endif //MYCOOLGAMEENGINE_MOUSE_H
