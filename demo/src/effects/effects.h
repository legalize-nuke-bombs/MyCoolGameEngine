//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_EFFECTS_H
#define MYCOOLGAMEENGINE_EFFECTS_H

#include <stdbool.h>

struct component_vtable;
struct effects;
struct effect;

const char* effects_component_key(void);

extern const struct component_vtable effects_vtable;

void effects_apply(struct effects *this, struct effect *effect);

struct effect* effects_find(const struct effects *this, const char *key);
bool effects_has(const struct effects *this, const char *key);

#endif //MYCOOLGAMEENGINE_EFFECTS_H
