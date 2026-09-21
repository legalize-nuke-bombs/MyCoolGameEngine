//
// Created by nikita on 22.09.2026.
//

#include "scene.h"

#include <stddef.h>
#include <stdio.h>

void scene_init(struct scene *this) {
    this->_name = "Default scene";
    this->_entity_collection = NULL;
}
void scene_destroy(const struct scene *this) {
    if (this->_entity_collection != NULL) {
        entity_collection_destroy(this->_entity_collection);
    }
}

const char* scene_get_name(struct scene *this) {
    return this->_name;
}
void scene_set_name(struct scene *this, const char *name) {
    this->_name = name;
}

struct entity_collection* scene_get_entity_collection(const struct scene *this) {
    return this->_entity_collection;
}
void scene_set_entity_collection(struct scene *this, struct entity_collection *entity_collection) {
    this->_entity_collection = entity_collection;
}

void scene_update(struct scene *this, const struct update_context *context) {
    if (this->_entity_collection != NULL) {
        entity_collection_update(this->_entity_collection, context);
    }
}

void scene_print(const struct scene *this) {
    printf("Scene %s\n", this->_name);
}