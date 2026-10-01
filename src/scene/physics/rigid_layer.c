//
// Created by nikita on 01.10.2026.
//

#include "rigid_layer.h"

#include <stdlib.h>

struct rigid_layer {
    char *name;
    uint8_t priority;
};

struct rigid_layer* rigid_layer_create(char *name, uint8_t priority) {
    struct rigid_layer* this = calloc(1, sizeof(struct rigid_layer));
    this->name = name;
    this->priority = priority;
    return this;
}
void rigid_layer_destroy(struct rigid_layer* this) {
    free(this->name);
    free(this);
}

const char* rigid_layer_get_name(const struct rigid_layer* this) {
    return this->name;
}
uint8_t rigid_layer_get_priority(const struct rigid_layer* this) {
    return this->priority;
}