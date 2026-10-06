//
// Created by Nikita on 02.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SIMULATOR_H
#define MYCOOLGAMEENGINE_SIMULATOR_H


struct component_vtable;
struct entity;

const char* simulator_component_key();

extern const struct component_vtable simulator_vtable;

#endif //MYCOOLGAMEENGINE_SIMULATOR_H
