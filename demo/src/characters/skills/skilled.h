//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILLED_H
#define MYCOOLGAMEENGINE_SKILLED_H

struct entity;
struct component_vtable;

const char* skilled_component_key(void);

extern const struct component_vtable skilled_vtable;

#endif //MYCOOLGAMEENGINE_SKILLED_H
