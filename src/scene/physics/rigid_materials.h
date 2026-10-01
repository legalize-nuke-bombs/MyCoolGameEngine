//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_MATERIALS_H
#define MYCOOLGAMEENGINE_RIGID_MATERIALS_H

struct rigid_materials;
struct rigid_material;

struct rigid_materials* rigid_materials_create();
void rigid_materials_destroy(struct rigid_materials* this);

void rigid_materials_clear(const struct rigid_materials* this);

void rigid_materials_capture(const struct rigid_materials* this, struct rigid_material* material);
struct rigid_material* rigid_materials_get(const struct rigid_materials* this, const char *name);

#endif //MYCOOLGAMEENGINE_RIGID_MATERIALS_H
