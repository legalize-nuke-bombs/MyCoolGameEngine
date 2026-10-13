//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CAMERA_H
#define MYCOOLGAMEENGINE_CAMERA_H

#include "../../../api.h"


struct component_vtable;
struct entity;
struct camera;

MCGE_API const char* camera_component_key(void);

MCGE_API extern const struct component_vtable camera_vtable;

MCGE_API double camera_get_visible_height(const struct camera* this);

MCGE_API double camera_get_visible_height_multiplier(const struct camera* this);
MCGE_API void camera_set_visible_height_multiplier(struct camera* this, double multiplier);

#endif //MYCOOLGAMEENGINE_CAMERA_H
