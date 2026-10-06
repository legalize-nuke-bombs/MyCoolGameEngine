//
// Created by nikita on 28.09.2026.
//

#include "prefab.h"
#include "../entity.h"

#include <stdlib.h>
#include <string.h>

#include "../entity_factory.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../logging/logger.h"
#include "../../utils/fields.h"

struct prefab {
    struct fields* fields;
    int usages;
};

// The entity is built once right away: this checks the fields where the prefab is declared, not on its first instance
struct prefab* prefab_create(struct fields *fields) {
    entity_destroy(entity_factory_produce(fields));

    struct prefab* this = calloc(1, sizeof(struct prefab));
    this->fields = fields_clone(fields);
    logger_debug("Prefab %s is creating...", prefab_get_name(this));
    return this;
}
void prefab_destroy(struct prefab* this) {
    logger_debug("Prefab %s is destroying (this prefab was instanced %d times!)...", prefab_get_name(this), this->usages);
    fields_destroy(this->fields);
    free(this);
}

static void prefab_destroy_item(void *item) {
    prefab_destroy(item);
}

static struct asset_storage prefabs = {
    ._key = "prefab",
    ._destroy_item = prefab_destroy_item
};

static void prefab_asset_on_add(struct fields *fields) {
    asset_storage_add(&prefabs, fields_get_string(fields, "name", NULL), prefab_create(fields));
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
    logger_debug("Prefab %s is instancing...", prefab_get_name(this));
    this->usages++;
    return entity_factory_produce(this->fields);
}

const char* prefab_get_name(const struct prefab* this) {
    return fields_get_string(this->fields, "name", "<unnamed>");
}

static bool prefab_fields_contain_component(struct fields *fields, const char* key, const enum entity_query query) {
    const struct fields_list *components = fields_get_list(fields, "components");
    for (int i = 0; i < fields_list_count(components); i++) {
        if (strcmp(fields_key(fields_list_get(components, i)), key) == 0) {
            return true;
        }
    }
    if (query == entity_query_recursive) {
        const struct fields_list *children = fields_get_list(fields, "children");
        for (int i = 0; i < fields_list_count(children); i++) {
            if (prefab_fields_contain_component(fields_list_get(children, i), key, query)) {
                return true;
            }
        }
    }
    return false;
}

bool prefab_contains_component(const struct prefab* this, const char* key, const enum entity_query query) {
    return prefab_fields_contain_component(this->fields, key, query);
}
