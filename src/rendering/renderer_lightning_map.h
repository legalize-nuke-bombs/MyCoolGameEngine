//
// Created by nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H
#define MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H

struct renderer_lightning_map;
struct renderer_pipeline;

struct renderer_lightning_map* renderer_lightning_map_create(struct renderer_pipeline *pipeline);
void renderer_lightning_map_destroy(struct renderer_lightning_map* this);

#endif //MYCOOLGAMEENGINE_RENDERER_LIGHTNING_MAP_H
