//
// Created by Nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_GAME_CLOSER_H
#define MYCOOLGAMEENGINE_GAME_CLOSER_H

struct entity;
struct component_vtable;

const char* game_closer_component_key(void);

extern const struct component_vtable game_closer_vtable;

#endif //MYCOOLGAMEENGINE_GAME_CLOSER_H
