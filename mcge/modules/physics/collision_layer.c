#include "collision_layer.h"

#include <stdlib.h>

struct collision_layer* collision_layer_create(const uint8_t index, const enum collision_response default_response) {
    struct collision_layer *this = calloc(1, sizeof(struct collision_layer));
    this->_index = index;
    this->_default_response = default_response;
    return this;
}
void collision_layer_destroy(struct collision_layer *this) {
    free(this);
}

uint8_t collision_layer_get_index(const struct collision_layer *this) {
    return this->_index;
}
enum collision_response collision_layer_get_default_response(const struct collision_layer *this) {
    return this->_default_response;
}

const struct collision_layer collision_layer_default = {
    ._index = 0,
    ._default_response = collision_response_block
};
