//
// Created by nikita on 22.09.2026.
//

#include "scene.h"

#include <stdlib.h>

#include "../logging/logger.h"

void scene_init(struct scene *this) {
    this->_name = "Default scene";
    logger_info("Scene %s is initializing...\n", this->_name);
    list_init(&this->_entities, 16);
}
void scene_destroy(const struct scene *this) {
    logger_info("Scene %s is destroying...\n", this->_name);
    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *entity = list_get(&this->_entities, i);
        entity_destroy(entity);
        free(entity);
    }
    list_destroy(&this->_entities);
}

const char* scene_get_name(const struct scene *this) {
    return this->_name;
}
void scene_set_name(struct scene *this, const char *name) {
    this->_name = name;
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    list_add(&this->_entities, entity);
}

void scene_update(const struct scene *this, const struct update_context *context) {
    for (int i = 0; i < list_count(&this->_entities); i++) {
        const struct entity *entity = list_get(&this->_entities, i);
        entity_update(entity, context);
    }
}