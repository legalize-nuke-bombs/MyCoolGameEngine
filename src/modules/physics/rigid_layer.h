//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_LAYER_H
#define MYCOOLGAMEENGINE_RIGID_LAYER_H
#include <stdint.h>

struct rigid_layer;
struct asset_type;

extern const struct asset_type rigid_layer_asset_type;

struct rigid_layer* rigid_layer_asset_get(const char *name);

uint8_t rigid_layer_get_priority(const struct rigid_layer* this);

#endif //MYCOOLGAMEENGINE_RIGID_LAYER_H
