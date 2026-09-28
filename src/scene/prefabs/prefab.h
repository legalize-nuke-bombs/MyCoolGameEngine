//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PREFAB_H
#define MYCOOLGAMEENGINE_PREFAB_H

struct prefab;
struct entity;

struct prefab* prefab_create(char* name, struct entity* entity);
void prefab_destroy(struct prefab* this);

struct entity* prefab_instantiate(struct prefab* this);

const char* prefab_get_name(const struct prefab* this);

#endif //MYCOOLGAMEENGINE_PREFAB_H
