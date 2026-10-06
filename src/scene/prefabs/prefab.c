//
// Created by nikita on 28.09.2026.
//

#include "prefab.h"
#include "../entity.h"

#include <stdlib.h>
#include <string.h>

#include "../entity_parser.h"
#include "../scene.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
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

static void prefab_destroy_item(void *item) {
    prefab_destroy(item);
}

static struct asset_storage prefabs = {
    ._key = "prefab",
    ._destroy_item = prefab_destroy_item
};

static void prefab_asset_on_add(struct parser *parser) {
    char *name = asset_storage_parse_name(&prefabs, parser);
    if (name == NULL) {
        return;
    }
    struct entity *entity = entity_parse(parser, strdup(name));
    asset_storage_add(&prefabs, name, prefab_create(strdup(name), entity));
}
static void prefab_asset_on_clear(void) {
    asset_storage_clear(&prefabs);
}
static void prefab_asset_on_destroy(void) {
    asset_storage_destroy(&prefabs);
}

const struct asset_type prefab_asset_type = {
    .key = "prefab",
    .on_add = prefab_asset_on_add,
    .on_clear = prefab_asset_on_clear,
    .on_destroy = prefab_asset_on_destroy
};

struct prefab* prefab_asset_get(const char *name) {
    return asset_storage_get(&prefabs, name);
}

struct entity* prefab_instantiate(struct prefab* this) {
    logger_debug("Prefab %s is instancing entity %s...", this->name, entity_get_name(this->entity));
    this->usages++;
    return entity_clone(this->entity);
}

const char* prefab_get_name(const struct prefab* this) {
    return this->name;
}
bool prefab_contains_component(const struct prefab* this, const char* key, const enum entity_query query) {
    return entity_try_get_component(this->entity, key, query);
}