//
// Created by nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H
#define MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H

struct light_map;
struct renderer_pipeline;

struct light_map* light_map_create(struct renderer_pipeline *pipeline);
void light_map_destroy(struct light_map* this);

#endif //MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H
