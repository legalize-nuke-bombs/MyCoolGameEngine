//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CAMERA_H
#define MYCOOLGAMEENGINE_CAMERA_H

#include "../../../api.h"


struct component_vtable;
struct entity;

MCGE_API const char* camera_component_key(void);

MCGE_API extern const struct component_vtable camera_vtable;

#endif //MYCOOLGAMEENGINE_CAMERA_H
