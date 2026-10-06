//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PREFAB_H
#define MYCOOLGAMEENGINE_PREFAB_H
#include <stdbool.h>

#include "../entity.h"

struct prefab;
struct entity;
struct fields;
struct asset_type;

extern const struct asset_type prefab_asset_type;

struct prefab* prefab_asset_get(const char *name);

struct prefab* prefab_create(struct fields *fields);
void prefab_destroy(struct prefab* this);

struct entity* prefab_instantiate(struct prefab* this);

const char* prefab_get_name(const struct prefab* this);
bool prefab_contains_component(const struct prefab* this, const char *key, enum entity_query query);

#endif //MYCOOLGAMEENGINE_PREFAB_H
