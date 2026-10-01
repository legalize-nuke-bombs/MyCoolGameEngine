//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_MATERIAL_H
#define MYCOOLGAMEENGINE_RIGID_MATERIAL_H

struct rigid_material;

struct rigid_material* rigid_material_create(char *name, double friction);
void rigid_material_destroy(struct rigid_material *layer);

const char* rigid_material_get_name(const struct rigid_material *layer);
double rigid_material_get_friction(const struct rigid_material *layer);

#endif //MYCOOLGAMEENGINE_RIGID_MATERIAL_H
