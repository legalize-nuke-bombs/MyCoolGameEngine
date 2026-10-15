//
// Created by nikita on 01.10.2026.
//

#include "rigid_material.h"

#include <stdlib.h>

#include "mcge/memory/allocator.h"

struct rigid_material* rigid_material_create(char *name, const double friction, const double restitution) {
    struct rigid_material *this = allocator_new0(1, sizeof(struct rigid_material));
    this->_name = name;
    this->_friction = friction;
    this->_restitution = restitution;
    return this;
}
void rigid_material_destroy(struct rigid_material *this) {
    allocator_delete(this->_name);
    allocator_delete(this);
}

const char* rigid_material_get_name(const struct rigid_material *this) {
    return this->_name;
}
double rigid_material_get_friction(const struct rigid_material *this) {
    return this->_friction;
}
double rigid_material_get_restitution(const struct rigid_material *this) {
    return this->_restitution;
}

const struct rigid_material rigid_material_default = {
    ._friction = 0.5f,
    ._restitution = 0.5f,
};
