#include "catalogs.h"

#include <stdlib.h>

#include "catalog.h"
#include "../logging/logger.h"
#include "../modules/bt/bt_graph.h"
#include "../rendering/renderer_layer.h"
#include "../rendering/texture.h"
#include "../modules/physics/rigid_layer.h"
#include "../modules/physics/rigid_material.h"
#include "../scene/prefabs/prefab.h"
#include "../msystems/msystem.h"
#include "../utils/dictionary.h"
#include "../utils/list.h"
#include "../utils/string_dictionary.h"


static struct {
    struct list list;
    struct dictionary *dict;
} catalogs;


static void catalogs_register(const struct catalog_vtable *vtable) {
    struct catalog *catalog = catalog_create(vtable);
    const char *key = catalog_get_key(catalog);
    if (dictionary_try_add(catalogs.dict, (void*)key, catalog)) {
        list_add(&catalogs.list, catalog);
        logger_debug("Catalogs know catalog %s", key);
    }
    else {
        logger_warn("Catalogs failed to register catalog %s", key);
        catalog_destroy(catalog);
    }
}

// Catalogs are cleared from the last one to the first one: register a catalog after the catalogs its items point to
static void catalogs_register_all(void) {
    catalogs_register(&texture_catalog_vtable);
    catalogs_register(&renderer_layer_catalog_vtable);
    catalogs_register(&rigid_layer_catalog_vtable);
    catalogs_register(&rigid_material_catalog_vtable);
    catalogs_register(&prefab_catalog_vtable);
    catalogs_register(&bt_graph_catalog_vtable);
    logger_info("Catalogs know %d catalogs", list_count(&catalogs.list));
}

static void catalogs_on_create(void) {
    catalogs.list = list_create(8);
    catalogs.dict = string_dictionary_build(3);
    catalogs_register_all();
}
static void catalogs_on_destroy(void) {
    for (int i = list_count(&catalogs.list) - 1; i >= 0; i--) {
        catalog_destroy(list_get(&catalogs.list, i));
    }
    list_destroy(&catalogs.list);
    dictionary_destroy(catalogs.dict);
}

static void catalogs_on_disable(void) {
    for (int i = list_count(&catalogs.list) - 1; i >= 0; i--) {
        catalog_clear(list_get(&catalogs.list, i));
    }
}

const struct msystem catalogs_msystem = {
    .name = "catalogs",
    .on_create = catalogs_on_create,
    .on_disable = catalogs_on_disable,
    .on_destroy = catalogs_on_destroy
};

struct catalog* catalogs_get(const char *key) {
    struct catalog *catalog = key != NULL ? dictionary_get(catalogs.dict, (void*)key) : NULL;
    if (catalog == NULL) {
        logger_warn("Catalogs do not know catalog `%s`", key ? key : "<null>");
    }
    return catalog;
}
void* catalogs_try_get_item(const char *key, const char *name) {
    const struct catalog *catalog = catalogs_get(key);
    if (catalog == NULL) {
        return NULL;
    }
    return catalog_get(catalog, name);
}
void* catalogs_get_item(const char *key, const char *name) {
    void *item = catalogs_try_get_item(key, name);
    if (item == NULL) {
        logger_warn("Catalog %s failed to find `%s`", key, name ? name : "<null>");
    }
    return item;
}
