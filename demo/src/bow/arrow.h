//
// Created by Nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_ARROW_H
#define MYCOOLGAMEENGINE_ARROW_H

#include "bowman_stats.h"

struct arrow;
struct character;

const char* arrow_component_key(void);

extern const struct component_vtable arrow_vtable;

void arrow_launch(struct arrow *this, struct character *target, struct bowman_stats stats);

struct bowman_stats arrow_stats(const struct arrow *this);

struct action* arrow_on_hit(struct arrow *this);

#endif //MYCOOLGAMEENGINE_ARROW_H
