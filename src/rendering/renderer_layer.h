//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_LAYER_H
#define MYCOOLGAMEENGINE_RENDERER_LAYER_H
#include <stdint.h>

struct renderer_layer;
struct asset_type;

extern const struct asset_type renderer_layer_asset_type;

struct renderer_layer* renderer_layer_asset_try_get(const char *name);

struct renderer_layer* renderer_layer_create(char *name, uint8_t priority);
void renderer_layer_destroy(struct renderer_layer *layer);

const char* renderer_layer_get_name(const struct renderer_layer *layer);
uint8_t renderer_layer_get_priority(const struct renderer_layer *layer);

#endif //MYCOOLGAMEENGINE_RENDERER_LAYER_H
