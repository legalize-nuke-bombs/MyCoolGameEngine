//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_BODY_H
#define MYCOOLGAMEENGINE_RIGID_BODY_H

struct component_vtable;
struct entity;
struct rigid_body;
struct vector2;

const char* rigid_body_component_key(void);

extern const struct component_vtable rigid_body_vtable;

void rigid_body_push(struct rigid_body *this, struct vector2 impulse);
void rigid_body_drive(struct rigid_body *this, struct vector2 impulse);

struct vector2 rigid_body_get_velocity(const struct rigid_body *this);
double rigid_body_get_mass(const struct rigid_body *this);

#endif //MYCOOLGAMEENGINE_RIGID_BODY_H
