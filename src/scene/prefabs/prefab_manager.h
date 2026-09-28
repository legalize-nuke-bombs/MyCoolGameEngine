//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PREFAB_MANAGER_H
#define MYCOOLGAMEENGINE_PREFAB_MANAGER_H

struct prefab_manager;
struct prefab;
struct entity;

struct prefab_manager* prefab_manager_create();
void prefab_manager_destroy(struct prefab_manager* this);

void prefab_manager_capture_prefab(const struct prefab_manager* this, struct prefab* prefab);
struct prefab* prefab_manager_try_get(const struct prefab_manager* this, const char* key);

void prefab_manager_clear(const struct prefab_manager* this);

#endif //MYCOOLGAMEENGINE_PREFAB_MANAGER_H
