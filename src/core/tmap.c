//
// Created by nikita on 22.09.2026.
//

#include "tmap.h"

#include <stdlib.h>

#include "../utils/string_dictionary.h"
#include "../utils/list.h"
#include "../logging/logger.h"


void tmap_init(struct tmap *this) {
    this->_dictionary = string_dictionary_build(10);
}
void tmap_destroy(const struct tmap *this) {
    for (int i = 0; i < dictionary_capacity(&this->_dictionary); i++) {
        const struct dictionary_node dictionary_node = dictionary_get_node(&this->_dictionary, i);
        if (dictionary_node.value != NULL) {
            list_destroy(dictionary_node.value);
            free(dictionary_node.value);
        }
    }
    dictionary_destroy(&this->_dictionary);
}

void tmap_register_component(struct tmap *this, struct component *component) {
    const char* component_key = component_get_key(component);

    if (dictionary_absent(&this->_dictionary, (void*) component_key)) {
        struct list* list = malloc(sizeof(struct list));
        list_init(list, 1);
        if (dictionary_try_add(&this->_dictionary, (void*) component_key, list)) {
            logger_debug("TMap now knows component key %s", component_key);
        }
        else {
            free(list);
            logger_error("Failed to populate tmap, tmap count %d, tmap capacity %d", dictionary_count(&this->_dictionary), dictionary_capacity(&this->_dictionary));
        }
    }

    struct list* list = dictionary_get(&this->_dictionary, (void*) component_key);
    if (list != NULL) {
        list_add(list, component);
        logger_debug("TMap registered sample of %s", component_key);
    }
    else {
        logger_error("Failed to populate tmap list for component key %s", component_key);
    }
}