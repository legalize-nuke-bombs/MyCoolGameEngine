#include "asset_storage.h"

#include <stdlib.h>

#include "../logging/logger.h"
#include "../utils/dictionary.h"
#include "../utils/parser.h"
#include "../utils/string_dictionary.h"


char* asset_storage_parse_name(const struct asset_storage *this, struct parser *parser) {
    char *name = parser_next_dup(parser);
    if (name == NULL) {
        logger_warn("Asset %s expected name, got end of file", this->_key);
    }
    return name;
}

void asset_storage_add(struct asset_storage *this, char *name, void *item) {
    if (this->_items == NULL) {
        this->_items = string_dictionary_build(4);
    }
    if (item != NULL && dictionary_try_add(this->_items, name, item)) {
        logger_debug("Asset %s captured %s", this->_key, name);
        return;
    }
    logger_warn("Asset %s failed to capture %s", this->_key, name);
    if (item != NULL) {
        this->_destroy_item(item);
    }
    free(name);
}

void* asset_storage_try_get(const struct asset_storage *this, const char *name) {
    if (this->_items == NULL || name == NULL) {
        return NULL;
    }
    return dictionary_get(this->_items, (void*)name);
}
void* asset_storage_get(const struct asset_storage *this, const char *name) {
    void *item = asset_storage_try_get(this, name);
    if (item == NULL) {
        logger_warn("Asset %s failed to find `%s`", this->_key, name ? name : "<null>");
    }
    return item;
}

void asset_storage_clear(struct asset_storage *this) {
    if (this->_items == NULL) {
        return;
    }
    struct dictionary_iterator iterator = dictionary_begin(this->_items);
    struct dictionary_node node;
    while (dictionary_next(this->_items, &iterator, &node)) {
        this->_destroy_item(node.value);
        free(node.key);
    }
    dictionary_clear(this->_items);
}
void asset_storage_destroy(struct asset_storage *this) {
    if (this->_items == NULL) {
        return;
    }
    asset_storage_clear(this);
    dictionary_destroy(this->_items);
    this->_items = NULL;
}
