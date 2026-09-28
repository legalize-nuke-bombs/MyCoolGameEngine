//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_COLOR_H
#define MYCOOLGAMEENGINE_COLOR_H

#include <stdint.h>

struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

extern const struct color color_black;
extern const struct color color_white;

#endif //MYCOOLGAMEENGINE_COLOR_H
