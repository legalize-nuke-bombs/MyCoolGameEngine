//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TMAP_H
#define MYCOOLGAMEENGINE_TMAP_H

#include "../components/component.h"


struct tmap;
struct dictionary;

struct tmap* tmap_create(struct scene *scene);
void tmap_destroy(struct tmap *this);

void tmap_update(const struct tmap *this, const struct update_context *context);

void tmap_clear(const struct tmap *this);

const struct dictionary* tmap_try_get_components(const struct tmap *this, const char *component_key);

#endif //MYCOOLGAMEENGINE_TMAP_H
