//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PHYSICS_H
#define MYCOOLGAMEENGINE_PHYSICS_H

struct physics* physics_create();
void physics_destroy(struct physics* this);

void physics_clear(const struct physics* this);

struct rigid_layers* physics_get_layers(const struct physics* this);
struct rigid_materials* physics_get_materials(const struct physics* this);

#endif //MYCOOLGAMEENGINE_PHYSICS_H
