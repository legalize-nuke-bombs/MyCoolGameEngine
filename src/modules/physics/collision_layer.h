#ifndef MYCOOLGAMEENGINE_COLLISION_LAYER_H
#define MYCOOLGAMEENGINE_COLLISION_LAYER_H

#include <stdint.h>

#include "collision_response.h"

struct collision_layer {
    uint8_t _index;
    enum collision_response _default_response;
};

struct collision_layer* collision_layer_create(uint8_t index, enum collision_response default_response);
void collision_layer_destroy(struct collision_layer *this);

uint8_t collision_layer_get_index(const struct collision_layer *this);
enum collision_response collision_layer_get_default_response(const struct collision_layer *this);

// The layer of the colliders that name no layer: index 0, blocks
extern const struct collision_layer collision_layer_default;

#endif //MYCOOLGAMEENGINE_COLLISION_LAYER_H
