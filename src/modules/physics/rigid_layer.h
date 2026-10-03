//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_LAYER_H
#define MYCOOLGAMEENGINE_RIGID_LAYER_H
#include <stdint.h>

struct rigid_layer;
struct catalog_vtable;

extern const struct catalog_vtable rigid_layer_catalog_vtable;

uint8_t rigid_layer_get_priority(const struct rigid_layer* this);

#endif //MYCOOLGAMEENGINE_RIGID_LAYER_H
