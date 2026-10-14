//
// Created by Nikita on 14.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CLICKABLE_H
#define MYCOOLGAMEENGINE_CLICKABLE_H

#include "mcge/api.h"

struct clickable;

const char* clickable_component_key(void);

extern const struct component_vtable clickable_vtable;

MCGE_API struct action* clickable_get_on_pressed(struct clickable *this);
MCGE_API struct action* clickable_get_on_released(struct clickable *this);

#endif //MYCOOLGAMEENGINE_CLICKABLE_H
