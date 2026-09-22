//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TMAP_H
#define MYCOOLGAMEENGINE_TMAP_H

#include "components/component.h"
#include "../utils/dictionary.h"
#include "../utils/list.h"


struct tmap {
    struct dictionary _dictionary;
};

void tmap_init(struct tmap *this);
void tmap_destroy(const struct tmap *this);

void tmap_register_component(struct tmap *this, struct component *component);

const struct list* tmap_try_get_components(const struct tmap *this, const char *component_key);

#endif //MYCOOLGAMEENGINE_TMAP_H
