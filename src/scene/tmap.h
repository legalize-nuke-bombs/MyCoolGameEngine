//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TMAP_H
#define MYCOOLGAMEENGINE_TMAP_H

#include "components/component.h"
#include "../utils/list.h"


struct tmap;

struct tmap* tmap_create(void);
void tmap_destroy(struct tmap *this);

void tmap_clear(const struct tmap *this);

void tmap_register_component(const struct tmap *this, struct component *component);

const struct list* tmap_try_get_components(const struct tmap *this, const char *component_key);

int tmap_remove_dead(const struct tmap *this);

#endif //MYCOOLGAMEENGINE_TMAP_H
