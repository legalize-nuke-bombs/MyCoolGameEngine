//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_LAYER_H
#define MYCOOLGAMEENGINE_RIGID_LAYER_H
#include <stdint.h>

struct rigid_layer;

struct rigid_layer* rigid_layer_create(uint8_t priority);
void rigid_layer_destroy(struct rigid_layer *this);

uint8_t rigid_layer_get_priority(const struct rigid_layer* this);

#endif //MYCOOLGAMEENGINE_RIGID_LAYER_H
