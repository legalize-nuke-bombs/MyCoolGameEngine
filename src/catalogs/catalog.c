#include "catalog.h"

#include <stdlib.h>

#include "../logging/logger.h"
#include "../utils/dictionary.h"
#include "../utils/parser.h"
#include "../utils/string_dictionary.h"


struct catalog {
    const struct catalog_vtable *vtable;
    struct dictionary *items;
};


struct catalog* catalog_create(const struct catalog_vtable *vtable) {
    logger_debug("Catalog %s is creating...", vtable->key());
    struct catalog *this = calloc(1, sizeof(struct catalog));
    this->vtable = vtable;
    this->items = string_dictionary_build(4);
    return this;
}
void catalog_destroy(struct catalog *this) {
    logger_debug("Catalog %s is destroying...", catalog_get_key(this));
    catalog_clear(this);
    dictionary_destroy(this->items);
    free(this);
}

void catalog_clear(struct catalog *this) {
    struct dictionary_iterator iterator = dictionary_begin(this->items);
    struct dictionary_node node;
    while (dictionary_next(this->items, &iterator, &node)) {
        this->vtable->on_destroy_item(node.value);
        free(node.key);
    }
    dictionary_clear(this->items);
}

const char* catalog_get_key(const struct catalog *this) {
    return this->vtable->key();
}

void catalog_add(struct catalog *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    char *name = parser_next_dup(parser);
    if (name == NULL) {
        logger_warn("Catalog %s expected item name, got end of file", catalog_get_key(this));
        return;
    }
    void *item = this->vtable->on_create_item(name, parser, subsystems);
    if (item != NULL && dictionary_try_add(this->items, name, item)) {
        logger_debug("Catalog %s captured %s", catalog_get_key(this), name);
        return;
    }
    logger_warn("Catalog %s failed to capture %s", catalog_get_key(this), name);
    if (item != NULL) {
        this->vtable->on_destroy_item(item);
    }
    free(name);
}

void* catalog_get(const struct catalog *this, const char *name) {
    if (name == NULL) {
        return NULL;
    }
    return dictionary_get(this->items, (void*)name);
}
