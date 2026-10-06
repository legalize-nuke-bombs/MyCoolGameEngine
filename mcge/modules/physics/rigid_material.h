//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_MATERIAL_H
#define MYCOOLGAMEENGINE_RIGID_MATERIAL_H

#include "../../api.h"

struct rigid_material;

struct rigid_material {
    double _friction;
    double _restitution;
};

MCGE_API struct rigid_material* rigid_material_create(double friction, double restitution);
MCGE_API void rigid_material_destroy(struct rigid_material *this);

MCGE_API double rigid_material_get_friction(const struct rigid_material *this);
MCGE_API double rigid_material_get_restitution(const struct rigid_material *this);

MCGE_API extern const struct rigid_material rigid_material_default;

#endif //MYCOOLGAMEENGINE_RIGID_MATERIAL_H
