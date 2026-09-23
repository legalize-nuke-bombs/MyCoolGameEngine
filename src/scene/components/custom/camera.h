//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CAMERA_H
#define MYCOOLGAMEENGINE_CAMERA_H

#include "../component.h"

struct camera;

struct camera* camera_create(struct entity *parent);

struct component* camera_as_component(struct camera *this);

#endif //MYCOOLGAMEENGINE_CAMERA_H
