//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_LAYER_H
#define MYCOOLGAMEENGINE_RENDERER_LAYER_H
#include <stdint.h>
#include "../api.h"

struct renderer_layer;

MCGE_API struct renderer_layer* renderer_layer_create(char *name, uint8_t priority);
MCGE_API void renderer_layer_destroy(struct renderer_layer *layer);

MCGE_API const char* renderer_layer_get_name(const struct renderer_layer *layer);
MCGE_API uint8_t renderer_layer_get_priority(const struct renderer_layer *layer);

#endif //MYCOOLGAMEENGINE_RENDERER_LAYER_H
