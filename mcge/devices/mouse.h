//
// Created by nikita on 13.10.2026.
//

#ifndef MYCOOLGAMEENGINE_MOUSE_H
#define MYCOOLGAMEENGINE_MOUSE_H

#include "../api.h"


MCGE_API extern const struct msystem mouse_msystem;

struct mouse_on_wheel_scrolled_callback_context {
    float dx;
    float dy;
};
MCGE_API struct action* mouse_get_on_wheel_scrolled();

#endif //MYCOOLGAMEENGINE_MOUSE_H
