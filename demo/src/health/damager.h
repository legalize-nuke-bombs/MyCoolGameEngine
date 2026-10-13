//
// Created by nikita on 12.10.2026.
//

#ifndef MYCOOLGAMEENGINE_DAMAGER_H
#define MYCOOLGAMEENGINE_DAMAGER_H

#include "damage.h"

struct damager;
struct health;

const char* damager_component_key(void);

extern const struct component_vtable damager_vtable;

void damager_deal_damage(const struct damager *this, struct health *target_health, struct damage damage);

struct action* damager_on_damage_dealt(struct damager *this);
struct action* damager_on_kill(struct damager *this);

#endif //MYCOOLGAMEENGINE_DAMAGER_H
