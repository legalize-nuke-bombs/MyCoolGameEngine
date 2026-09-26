//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_LIGHT_MAP_H
#define MYCOOLGAMEENGINE_LIGHT_MAP_H

#include "../../utils/rect.h"
#include "../../utils/color.h"


struct light_map;

struct renderer_primitive* light_map_create();
void light_map_destroy(struct light_map* this);

struct light_map_draw_call {
    struct rect rect;
    struct renderer_primitive* primitive;
};

void light_map_draw_primitive(struct light_map* this, struct light_map_draw_call draw_call);

void light_map_reset(struct light_map* this);

void light_map_set_darkness_color(struct light_map* this, struct color color);
void light_map_reset_darkness_color(struct light_map* this);

#endif //MYCOOLGAMEENGINE_LIGHT_MAP_H
