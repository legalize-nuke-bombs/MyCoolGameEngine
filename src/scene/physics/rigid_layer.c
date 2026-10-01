//
// Created by nikita on 01.10.2026.
//

#include "rigid_layer.h"

#include <stdlib.h>


struct rigid_layer {
    char *name;
    double friction;
};


struct rigid_layer* rigid_layer_create(char *name, double friction) {
    struct rigid_layer *layer = calloc(1, sizeof(struct rigid_layer));
    layer->name = name;
    layer->friction = friction;
    return layer;
}
void rigid_layer_destroy(struct rigid_layer *layer) {
    free(layer->name);
    free(layer);
}

const char* rigid_layer_get_name(const struct rigid_layer *layer) {
    return layer->name;
}
double rigid_layer_get_friction(const struct rigid_layer *layer) {
    return layer->friction;
}