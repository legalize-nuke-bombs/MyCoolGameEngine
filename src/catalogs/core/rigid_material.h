//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_MATERIAL_H
#define MYCOOLGAMEENGINE_RIGID_MATERIAL_H

struct rigid_material;
struct catalog_vtable;

struct rigid_material {
    double _friction;
    double _restitution;
};

extern const struct catalog_vtable rigid_material_catalog_vtable;

double rigid_material_get_friction(const struct rigid_material *this);
double rigid_material_get_restitution(const struct rigid_material *this);

extern const struct rigid_material rigid_material_default;

#endif //MYCOOLGAMEENGINE_RIGID_MATERIAL_H
