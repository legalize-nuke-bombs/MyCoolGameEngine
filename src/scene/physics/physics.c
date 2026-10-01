//
// Created by nikita on 01.10.2026.
//

#include "physics.h"

#include <stdlib.h>

#include "rigid_layers.h"
#include "rigid_materials.h"


struct physics {
    struct rigid_layers *layers;
    struct rigid_materials *materials;
};


struct physics* physics_create() {
    struct physics* this = calloc(1, sizeof(struct physics));
    this->layers = rigid_layers_create();
    this->materials = rigid_materials_create();
    return this;
}
void physics_destroy(struct physics* this) {
    rigid_materials_destroy(this->materials);
    rigid_layers_destroy(this->layers);
    free(this);
}

void physics_clear(const struct physics* this) {
    rigid_materials_clear(this->materials);
    rigid_layers_clear(this->layers);
}

struct rigid_layers* physics_get_layers(const struct physics* this) {
    return this->layers;
}
struct rigid_materials* physics_get_materials(const struct physics* this) {
    return this->materials;
}