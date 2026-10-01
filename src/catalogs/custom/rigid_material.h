//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_MATERIAL_H
#define MYCOOLGAMEENGINE_RIGID_MATERIAL_H

struct rigid_material;
struct catalog_vtable;

extern const struct catalog_vtable rigid_material_catalog_vtable;

double rigid_material_get_friction(const struct rigid_material *this);

#endif //MYCOOLGAMEENGINE_RIGID_MATERIAL_H
