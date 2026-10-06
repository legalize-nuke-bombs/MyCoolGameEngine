//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_MANA_H
#define MYCOOLGAMEENGINE_MANA_H
#include <stdbool.h>

struct entity;
struct component_vtable;
struct mana;

const char* mana_component_key(void);

extern const struct component_vtable mana_vtable;

bool mana_try_take(struct mana *this, double amount);
double mana_amount(const struct mana *this);
double mana_max_amount(const struct mana *this);
double mana_percent(const struct mana *this);

#endif //MYCOOLGAMEENGINE_MANA_H
