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
};

struct prefab* prefab_create(char* name, struct entity* entity) {
    struct prefab* this = calloc(1, sizeof(struct prefab));
    this->name = name;
    this->entity = entity;
    return this;
}
void prefab_destroy(struct prefab* this) {
    free(this->name);
    entity_destroy(this->entity);
    free(this);
}

struct entity* prefab_instantiate(const struct prefab* this) {
    logger_debug("Prefab %s is instancing entity %s...", this->name, entity_get_name(this->entity));
    return entity_clone(this->entity);
}