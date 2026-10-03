//
// Created by nikita on 28.09.2026.
//

#include "prefab.h"
#include "../entity.h"

#include <stdlib.h>
#include <string.h>

#include "../entity_parser.h"
#include "../scene.h"
#include "../../catalogs/catalog.h"
#include "../../logging/logger.h"
#include "../../subsystems/subsystem_collection.h"

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

static const char* prefab_catalog_key(void) {
    return "prefab";
}

static void* prefab_on_create_item(const char *name, struct parser *parser, const struct subsystem_collection *subsystems) {
    struct scene *scene = (struct scene*)subsystem_collection_get(subsystems, "scene");
    struct entity *entity = entity_parse(parser, scene, strdup(name));
    return prefab_create(strdup(name), entity);
}
static void prefab_on_destroy_item(void *item) {
    prefab_destroy(item);
}

const struct catalog_vtable prefab_catalog_vtable = {
    .key = prefab_catalog_key,
    .on_create_item = prefab_on_create_item,
    .on_destroy_item = prefab_on_destroy_item
};

struct entity* prefab_instantiate(struct prefab* this) {
    logger_debug("Prefab %s is instancing entity %s...", this->name, entity_get_name(this->entity));
    this->usages++;
    return entity_clone(this->entity);
}

const char* prefab_get_name(const struct prefab* this) {
    return this->name;
}