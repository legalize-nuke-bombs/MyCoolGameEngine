//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TMAP_H
#define MYCOOLGAMEENGINE_TMAP_H

#include "components/component.h"
#include "../utils/dictionary.h"

struct tmap {
    struct dictionary _dictionary;
};

void tmap_init(struct tmap *this);
void tmap_destroy(const struct tmap *this);

void tmap_register_component(struct tmap *this, struct component *component);

#endif //MYCOOLGAMEENGINE_TMAP_H
