//
// Created by nikita on 03.10.2026.
//

#include <stdlib.h>

#include "factory_internal.h"
#include "../logging/logger.h"
#include "../utils/string_dictionary.h"


void factory_base_create(struct factory *this, const struct factory_vtable *vtable) {
    this->vtable = vtable;
    this->dict = string_dictionary_build(5);
}

void factory_register(const struct factory *this, const char *key, void *constructor) {
    if (dictionary_try_add(this->dict, (void*)key, constructor)) {
        logger_debug("Factory %s registered %s", this->vtable->key, key);
    }
    else {
        logger_error("Factory %s failed to register %s", this->vtable->key, key);
    }
}

void* factory_get_constructor(const struct factory *this, const char *key) {
    void* constructor = dictionary_get(this->dict, (void*)key);
    if (constructor == NULL) {
        logger_warn("Factory %s does not know %s", this->vtable->key, key);
        return NULL;
    }
    return constructor;
}

void factory_destroy(struct factory *this) {
    dictionary_destroy(this->dict);
    free(this);
}

const char* factory_get_key(const struct factory *this) {
    return this->vtable->key;
}