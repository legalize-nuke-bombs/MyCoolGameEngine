//
// Created by nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HANDS_H
#define MYCOOLGAMEENGINE_HANDS_H

#include "hands_action.h"

struct hands;

const char* hands_component_key();

extern const struct component_vtable hands_vtable;

bool hands_try_put(struct hands* this, struct hands_action action);

#endif //MYCOOLGAMEENGINE_HANDS_H
