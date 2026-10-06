//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PREFAB_H
#define MYCOOLGAMEENGINE_PREFAB_H
#include <stdbool.h>

#include "../entity.h"
#include "../../api.h"

struct prefab;
struct entity;
struct fields;

MCGE_API struct prefab* prefab_create(struct fields *fields);
MCGE_API void prefab_destroy(struct prefab* this);

MCGE_API struct entity* prefab_instantiate(struct prefab* this);

MCGE_API const char* prefab_get_name(const struct prefab* this);
MCGE_API bool prefab_contains_component(const struct prefab* this, const char *key, enum entity_query query);

#endif //MYCOOLGAMEENGINE_PREFAB_H
