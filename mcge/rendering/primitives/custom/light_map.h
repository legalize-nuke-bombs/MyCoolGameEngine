//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_LIGHT_MAP_H
#define MYCOOLGAMEENGINE_LIGHT_MAP_H

#include <stdbool.h>

#include "../../../utils/rect.h"
#include "../../../utils/color.h"
#include "../../../api.h"


struct light_map;

MCGE_API struct renderer_primitive* light_map_create();
MCGE_API void light_map_destroy(struct light_map* this);

struct light_map_draw_call {
    struct rect rect;
    struct renderer_primitive* primitive;
};

MCGE_API void light_map_draw_primitive(struct light_map* this, struct light_map_draw_call draw_call);
MCGE_API void light_map_clear_draw_calls(struct light_map* this);

MCGE_API void light_map_reset(struct light_map* this);

MCGE_API void light_map_set_darkness_color(struct light_map* this, struct color color);
MCGE_API void light_map_reset_darkness_color(struct light_map* this);

MCGE_API void light_map_set_enable(struct light_map* this, bool value);
MCGE_API void light_map_reset_enable(struct light_map* this);

#endif //MYCOOLGAMEENGINE_LIGHT_MAP_H
