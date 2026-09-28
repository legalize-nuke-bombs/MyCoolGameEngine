//
// Created by nikita on 28.09.2026.
//

#include "prefab.h"
#include "../entity.h"

#include <stdlib.h>

#include "../../logging/logger.h"

struct prefab {
    char* name;
    struct entity* entity;
    int usages;
};

struct prefab* prefab_create(char* name, struct entity* entity) {
    logger_debug("Prefab %s is creating...", name);
    struct prefab* this = calloc(1, sizeof(struct prefab));
    this->name = name;
    this->entity = entity;
    return this;
}
void prefab_destroy(struct prefab* this) {
    logger_debug("Prefab %s is destroying (this prefab was instanced %d times!)...", this->name, this->usages);
    free(this->name);
    entity_destroy(this->entity);
    free(this);
}

struct entity* prefab_instantiate(struct prefab* this) {
    logger_debug("Prefab %s is instancing entity %s...", this->name, entity_get_name(this->entity));
    this->usages++;
    return entity_clone(this->entity);
}

const char* prefab_get_name(const struct prefab* this) {
    return this->name;
}