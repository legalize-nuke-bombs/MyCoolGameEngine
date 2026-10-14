//
// Created by nikita on 01.10.2026.
//

#include "rigid_layer.h"

#include <stdlib.h>

struct rigid_layer {
    uint8_t priority;
};

struct rigid_layer* rigid_layer_create(const uint8_t priority) {
    struct rigid_layer* this = calloc(1, sizeof(struct rigid_layer));
    this->priority = priority;
    return this;
}
void rigid_layer_destroy(struct rigid_layer *this) {
    free(this);
}

uint8_t rigid_layer_get_priority(const struct rigid_layer* this) {
    return this->priority;
}
