//
// Created by Nikita on 04.10.2026.
//

#ifndef MYCOOLGAMEENGINE_EFFECTS_H
#define MYCOOLGAMEENGINE_EFFECTS_H
#include <stdbool.h>

struct entity;
struct component_vtable;
struct effects;

enum effect {
    effect_fear
};
#define EFFECTS_NUM 2

const char* effects_component_key(void);

extern const struct component_vtable effects_vtable;

void effects_set_effect(struct effects *this, enum effect effect, double length);
double effects_get_effect(const struct effects *this, enum effect effect);
bool effects_has_effect(const struct effects *this, enum effect effect);

#endif //MYCOOLGAMEENGINE_EFFECTS_H
