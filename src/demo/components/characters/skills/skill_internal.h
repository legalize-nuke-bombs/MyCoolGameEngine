//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILL_INTERNAL_H
#define MYCOOLGAMEENGINE_SKILL_INTERNAL_H

#include <stdbool.h>

#include "skill.h"

struct skill_vtable {
    const char* key;
    void (*on_destroy)(struct skill *this);
    void (*on_enable)(struct skill *this);
    bool (*on_invoke)(struct skill *this);
};

void skill_base_create(struct skill *this, const struct skill_vtable *vtable, double manacost, double cooldown, struct entity *self);

#endif //MYCOOLGAMEENGINE_SKILL_INTERNAL_H
