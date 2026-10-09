//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BOWMAN_H
#define MYCOOLGAMEENGINE_BOWMAN_H

#include "bowman_stats.h"

struct bowman;

const char* bowman_component_key(void);

extern const struct component_vtable bowman_vtable;

#endif //MYCOOLGAMEENGINE_BOWMAN_H
