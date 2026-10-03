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
        logger_debug("Fabric %s registered %s", this->vtable->key, key);
    }
    else {
        logger_error("Fabric %s failed to register %s", this->vtable->key, key);
    }
}

void factory_destroy(struct factory *this) {
    dictionary_destroy(this->dict);
    free(this);
}

void* factory_produce(struct factory *this, const char *key) {
    return this->vtable->on_produce(this, key);
}