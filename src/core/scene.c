//
// Created by nikita on 22.09.2026.
//

#include "scene.h"
#include "../logging/logger.h"
#include "../utils/string_dictionary.h"

#include <stdlib.h>



void scene_init(struct scene *this) {
    this->_name = "Default scene";
    logger_info("Scene %s is initializing...", this->_name);
    list_init(&this->_entities, 16);
    this->_tmap = string_dictionary_build(10);
}
void scene_destroy(const struct scene *this) {
    logger_info("Scene %s is destroying...", this->_name);

    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *entity = list_get(&this->_entities, i);
        entity_destroy(entity);
        free(entity);
    }
    list_destroy(&this->_entities);

    for (int i = 0; i < dictionary_capacity(&this->_tmap); i++) {
        const struct dictionary_node dictionary_node = dictionary_get_node(&this->_tmap, i);
        if (dictionary_node.value != NULL) {
            list_destroy(dictionary_node.value);
            free(dictionary_node.value);
        }
    }
    dictionary_destroy(&this->_tmap);
}

const char* scene_get_name(const struct scene *this) {
    return this->_name;
}
void scene_set_name(struct scene *this, const char *name) {
    this->_name = name;
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    logger_debug("Scene %s is capturing entity %s", this->_name, entity_get_name(entity));

    list_add(&this->_entities, entity);

    for (int i = 0; i < entity_get_components_count(entity); i++) {
        struct component *component = entity_get_component(entity, i);
        const char* component_key = component_get_key(component);

        if (dictionary_absent(&this->_tmap, (void*) component_key)) {
            struct list* list = malloc(sizeof(struct list));
            list_init(list, 1);
            if (dictionary_try_add(&this->_tmap, (void*) component_key, list)) {
                logger_debug("TMap now knows component key %s", component_key);
            }
            else {
                free(list);
                logger_error("Failed to populate tmap, tmap count %d, tmap capacity %d", dictionary_count(&this->_tmap), dictionary_capacity(&this->_tmap));
            }
        }

        struct list* list = dictionary_get(&this->_tmap, (void*) component_key);
        if (list != NULL) {
            list_add(list, component);
            logger_debug("TMap registered sample of %s", component_key);
        }
        else {
            logger_error("Failed to populate tmap list for component key %s", component_key);
        }
    }
}

void scene_update(const struct scene *this, const struct update_context *context) {
    for (int i = 0; i < list_count(&this->_entities); i++) {
        const struct entity *entity = list_get(&this->_entities, i);
        entity_update(entity, context);
    }
}