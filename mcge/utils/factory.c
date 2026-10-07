#include "factory.h"

#include <stddef.h>

#include "dictionary.h"
#include "string_dictionary.h"
#include "../logging/logger.h"


struct factory factory_create(const char *name) {
    const struct factory this = {
        ._name = name,
        ._items = string_dictionary_build(5)
    };
    return this;
}
void factory_destroy(struct factory *this) {
    dictionary_destroy(this->_items);
    this->_items = NULL;
}

void factory_register(struct factory *this, const char *key, void *item) {
    if (dictionary_try_add(this->_items, (void*)key, item)) {
        logger_info("Factory `%s` knows `%s`", this->_name, key);
    }
    else {
        logger_error("Factory %s failed to register %s", this->_name, key);
    }
}

void* factory_find(const struct factory *this, const char *key) {
    void *item = key != NULL ? dictionary_get(this->_items, (void*)key) : NULL;
    if (item == NULL) {
        logger_warn("Factory %s does not know %s", this->_name, key ? key : "<null>");
    }
    return item;
}
