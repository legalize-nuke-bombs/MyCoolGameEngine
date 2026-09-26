//
// Created by nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H
#define MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H

#include "../../utils/rect.h"

struct light_map;
struct renderer_pipeline;
struct renderer_primitive;

struct light_map* light_map_create(struct renderer_pipeline *pipeline);
void light_map_destroy(struct light_map* this);

struct light_map_draw_call {
    struct rect rect;
    struct renderer_primitive* primitive;
};

void light_map_draw_primitive(struct light_map* this, struct light_map_draw_call draw_call);

#endif //MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H
