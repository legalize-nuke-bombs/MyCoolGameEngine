//
// Created by Nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CHARACTER_H
#define MYCOOLGAMEENGINE_CHARACTER_H

#include "character_group.h"

struct character;

const char* character_component_key(void);

extern const struct component_vtable character_vtable;

enum character_group character_get_group(const struct character *this);

#endif //MYCOOLGAMEENGINE_CHARACTER_H
