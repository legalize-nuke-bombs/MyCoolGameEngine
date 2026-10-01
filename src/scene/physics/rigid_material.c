//
// Created by nikita on 01.10.2026.
//

#include "rigid_material.h"

#include <stdlib.h>


struct rigid_material {
    char *name;
    double friction;
};


struct rigid_material* rigid_material_create(char *name, double friction) {
    struct rigid_material *layer = calloc(1, sizeof(struct rigid_material));
    layer->name = name;
    layer->friction = friction;
    return layer;
}
void rigid_material_destroy(struct rigid_material *layer) {
    free(layer->name);
    free(layer);
}

const char* rigid_material_get_name(const struct rigid_material *layer) {
    return layer->name;
}
double rigid_material_get_friction(const struct rigid_material *layer) {
    return layer->friction;
}