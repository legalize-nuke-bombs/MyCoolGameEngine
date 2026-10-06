//
// Created by nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_LIGHT_H
#define MYCOOLGAMEENGINE_BOX_LIGHT_H

struct box_light;
struct entity;
struct component_vtable;

const char* box_light_component_key(void);

extern const struct component_vtable box_light_vtable;

#endif //MYCOOLGAMEENGINE_BOX_LIGHT_H
