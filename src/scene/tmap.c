//
// Created by nikita on 22.09.2026.
//

#include "tmap.h"

#include <stdlib.h>

#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"
#include "../utils/list.h"
#include "../logging/logger.h"


struct tmap {
    struct dictionary *dictionary;
};

struct tmap* tmap_create(void) {
    struct tmap *this = malloc(sizeof(struct tmap));
    this->dictionary = string_dictionary_build(10);
    return this;
}
void tmap_destroy(struct tmap *this) {
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node dictionary_node = dictionary_get_node(this->dictionary, i);
        if (dictionary_node.value != NULL) {
            list_destroy(dictionary_node.value);
        }
    }
    dictionary_destroy(this->dictionary);
    free(this);
}

void tmap_register_component(const struct tmap *this, struct component *component) {
    const char* component_key = component_get_key(component);

    if (dictionary_absent(this->dictionary, (void*) component_key)) {
        struct list* list = list_create(1);
        if (dictionary_try_add(this->dictionary, (void*) component_key, list)) {
            logger_debug("TMap now knows component key %s", component_key);
        }
        else {
            list_destroy(list);
            logger_error("Failed to populate tmap, tmap count %d, tmap capacity %d", dictionary_count(this->dictionary), dictionary_capacity(this->dictionary));
        }
    }

    struct list* list = dictionary_get(this->dictionary, (void*) component_key);
    if (list != NULL) {
        list_add(list, component);
        logger_debug("TMap registered sample of %s", component_key);
    }
    else {
        logger_error("Failed to populate tmap list for component key %s", component_key);
    }
}

const struct list* tmap_try_get_components(const struct tmap *this, const char *component_key) {
    return dictionary_get(this->dictionary, (void*) component_key);
}

void tmap_remove_dead(const struct tmap *this) {
    int ctr = 0;
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node dictionary_node = dictionary_get_node(this->dictionary, i);
        const struct list* list = dictionary_node.value;
        if (list == NULL) {
            continue;
        }

        for (int j = 0; j < list_count(list); j++) {
            const struct component *component = list_get(list, j);
            if (component == NULL) {
                continue;
            }
            if (!component_is_alive(component)) {
                list_set(list, j, NULL);
                ctr++;
            }
        }
    }
    logger_debug("TMap removed %d dead components", ctr);
}