#ifndef MYCOOLGAMEENGINE_COLLISION_LAYER_H
#define MYCOOLGAMEENGINE_COLLISION_LAYER_H

#include <stdint.h>

#include "collision_response.h"
#include "../../api.h"

struct collision_layer {
    uint8_t _index;
    enum collision_response _default_response;
};

MCGE_API struct collision_layer* collision_layer_create(uint8_t index, enum collision_response default_response);
MCGE_API void collision_layer_destroy(struct collision_layer *this);

MCGE_API uint8_t collision_layer_get_index(const struct collision_layer *this);
MCGE_API enum collision_response collision_layer_get_default_response(const struct collision_layer *this);

MCGE_API extern const struct collision_layer collision_layer_default;

#endif //MYCOOLGAMEENGINE_COLLISION_LAYER_H
