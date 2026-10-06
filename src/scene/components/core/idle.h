//
// Created by nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_IDLE_H
#define MYCOOLGAMEENGINE_IDLE_H

struct entity;
struct component_vtable;

const char* idle_component_key(void);

extern const struct component_vtable idle_vtable;

#endif //MYCOOLGAMEENGINE_IDLE_H
