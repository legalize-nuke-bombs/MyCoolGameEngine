//
// Created by Nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PLAYER_H
#define MYCOOLGAMEENGINE_PLAYER_H

struct entity;
struct component_vtable;

const char* player_component_key(void);

extern const struct component_vtable player_vtable;

#endif //MYCOOLGAMEENGINE_PLAYER_H
