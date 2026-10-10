//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_COLOR_H
#define MYCOOLGAMEENGINE_COLOR_H

#include <stdint.h>
#include "../api.h"

struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

MCGE_API extern const struct color color_black;

MCGE_API extern const struct color color_white;

MCGE_API extern const struct color color_green;

MCGE_API extern const struct color color_red;
MCGE_API extern const struct color color_red_light;


#endif //MYCOOLGAMEENGINE_COLOR_H
