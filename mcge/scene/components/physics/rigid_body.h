//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_BODY_H
#define MYCOOLGAMEENGINE_RIGID_BODY_H

#include "../../../api.h"
#include "../../../utils/vector2.h"

struct component_vtable;
struct entity;
struct rigid_body;
struct vector2;

MCGE_API const char* rigid_body_component_key(void);

MCGE_API extern const struct component_vtable rigid_body_vtable;

MCGE_API void rigid_body_push(struct rigid_body *this, struct vector2 impulse);
MCGE_API void rigid_body_drive(struct rigid_body *this, struct vector2 target_velocity);

MCGE_API struct vector2 rigid_body_get_velocity(const struct rigid_body *this);
MCGE_API double rigid_body_get_mass(const struct rigid_body *this);

MCGE_API struct action* rigid_body_get_on_drive_intent(struct rigid_body *this);

struct rigid_body_on_movement_callback_context {
    const struct rigid_surface *surface;
    const struct vector2 step;
};
MCGE_API struct action* rigid_body_get_on_movement(struct rigid_body *this);

#endif //MYCOOLGAMEENGINE_RIGID_BODY_H
