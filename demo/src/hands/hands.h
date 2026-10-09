//
// Created by nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HANDS_H
#define MYCOOLGAMEENGINE_HANDS_H

#include "hands_action.h"
#include "hands_action_priorities.h"

struct hands;

const char* hands_component_key();

extern const struct component_vtable hands_vtable;

bool hands_try_put(struct hands* this, struct hands_action action);

double hands_get_current_action_timer(const struct hands *this);
double hands_get_current_action_cooldown(const struct hands *this);
double hands_get_current_action_scale(const struct hands *this);

struct action* hands_on_changed(struct hands* this);

#endif //MYCOOLGAMEENGINE_HANDS_H
