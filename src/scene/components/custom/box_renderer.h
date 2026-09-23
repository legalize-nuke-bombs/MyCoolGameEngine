//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_RENDERER_H
#define MYCOOLGAMEENGINE_BOX_RENDERER_H

#include "../../../utils/color.h"


struct box_renderer;

struct box_renderer* box_renderer_create(struct color color);

struct component * box_renderer_as_component(struct box_renderer *this);

#endif //MYCOOLGAMEENGINE_BOX_RENDERER_H
