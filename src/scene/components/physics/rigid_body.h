//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_BODY_H
#define MYCOOLGAMEENGINE_RIGID_BODY_H

struct parser;
struct entity;
struct rigid_body;
struct vector2;
struct chunks;

#include "../../../utils/rect.h"

const char* rigid_body_component_key(void);

struct component* rigid_body_create(struct parser *parser, struct entity *parent);

void rigid_body_push(struct rigid_body *this, struct vector2 impulse);
void rigid_body_drive(struct rigid_body *this, struct vector2 impulse);

struct vector2 rigid_body_get_velocity(const struct rigid_body *this);
double rigid_body_get_mass(const struct rigid_body *this);

void rigid_body_explosion(struct vector2 position, double impulse_at_one_meter, const struct chunks *chunks);

#endif //MYCOOLGAMEENGINE_RIGID_BODY_H
