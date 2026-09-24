//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CAMERA_H
#define MYCOOLGAMEENGINE_CAMERA_H

#include "../component.h"

struct camera;
struct parser;

const char* camera_component_key(void);

struct component* camera_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_CAMERA_H
