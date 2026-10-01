#include "catalogs.h"

#include <stdlib.h>

#include "catalog.h"
#include "../logging/logger.h"
#include "../subsystems/subsystem_internal.h"
#include "../utils/dictionary.h"
#include "../utils/list.h"
#include "../utils/string_dictionary.h"


struct catalogs {
    struct subsystem base;

    struct list list;
    struct dictionary *dict;
};


static const char* catalogs_get_name() {
    return "catalogs";
}

static void catalogs_on_destroy(struct subsystem *base);
static void catalogs_on_disable(struct subsystem *base);

static struct subsystem_vtable catalogs_vtable = {
    .name = catalogs_get_name,
    .on_destroy = catalogs_on_destroy,
    .on_enable = NULL,
    .on_disable = catalogs_on_disable
};


static void catalogs_register(struct catalogs *this, const struct catalog_vtable *vtable) {
    struct catalog *catalog = catalog_create(vtable);
    const char *key = catalog_get_key(catalog);
    if (dictionary_try_add(this->dict, (void*)key, catalog)) {
        list_add(&this->list, catalog);
        logger_debug("Catalogs know catalog %s", key);
    }
    else {
        logger_warn("Catalogs failed to register catalog %s", key);
        catalog_destroy(catalog);
    }
}

// Catalogs are cleared from the last one to the first one: register a catalog after the catalogs its items point to
static void catalogs_register_all(struct catalogs *this) {
}

struct subsystem* catalogs_create(const struct subsystem_collection *subsystems) {
    struct catalogs *this = calloc(1, sizeof(struct catalogs));
    struct subsystem *base = (struct subsystem*)this;
    subsystem_create(base, &catalogs_vtable, subsystems);

    this->list = list_create(8);
    this->dict = string_dictionary_build(3);
    catalogs_register_all(this);

    return base;
}
static void catalogs_on_destroy(struct subsystem *base) {
    struct catalogs *this = (struct catalogs*)base;
    for (int i = list_count(&this->list) - 1; i >= 0; i--) {
        catalog_destroy(list_get(&this->list, i));
    }
    list_destroy(&this->list);
    dictionary_destroy(this->dict);
}

static void catalogs_on_disable(struct subsystem *base) {
    const struct catalogs *this = (struct catalogs*)base;
    for (int i = list_count(&this->list) - 1; i >= 0; i--) {
        catalog_clear(list_get(&this->list, i));
    }
}

struct catalog* catalogs_get(const struct catalogs *this, const char *key) {
    struct catalog *catalog = key != NULL ? dictionary_get(this->dict, (void*)key) : NULL;
    if (catalog == NULL) {
        logger_warn("Catalogs do not know catalog `%s`", key ? key : "<null>");
    }
    return catalog;
}
void* catalogs_get_item(const struct catalogs *this, const char *key, const char *name) {
    const struct catalog *catalog = catalogs_get(this, key);
    if (catalog == NULL) {
        return NULL;
    }
    void *item = catalog_get(catalog, name);
    if (item == NULL) {
        logger_warn("Catalog %s failed to find `%s`", key, name ? name : "<null>");
    }
    return item;
}
