//
// Created by Nikita on 07.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HEALTH_H
#define MYCOOLGAMEENGINE_HEALTH_H

#include <stdbool.h>

#include "damage.h"

struct health;

const char* health_component_key(void);

extern const struct component_vtable health_vtable;

double health_amount(const struct health *this);
double health_max_amount(const struct health *this);
double health_scale(const struct health *this);
bool health_is_alive(const struct health* this);

void health_take_damage(struct health* this, struct damage damage);
void health_restore(struct health* this, double amount);

void health_resurrect(struct health* this, double amount);

void health_set_max_amount(struct health* this, double max_amount);

struct action* health_get_on_changed(struct health *this);
struct action* health_get_on_take_damage(struct health *this);
struct action* health_get_on_restore(struct health *this);
struct action* health_get_on_death(struct health *this);
struct action* health_get_on_resurrect(struct health *this);

#endif //MYCOOLGAMEENGINE_HEALTH_H
