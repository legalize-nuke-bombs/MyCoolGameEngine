//
// Created by nikita on 22.09.2026.
//

#include "scene.h"
#include "../logging/logger.h"

#include <stdlib.h>

#include "entity.h"
#include "../utils/action.h"


void scene_init(struct scene *this) {
    this->_name = "Default scene";
    logger_info("Scene %s is initializing...", this->_name);
    list_init(&this->_entities, 16);
    tmap_init(&this->_tmap);
}
void scene_awake(const struct scene *this) {
    logger_info("Scene %s is awaking...", this->_name);
    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *entity = list_get(&this->_entities, i);
        entity_awake(entity);
    }
}
void scene_destroy(const struct scene *this) {
    logger_info("Scene %s is destroying...", this->_name);

    for (int i = 0; i < list_count(&this->_entities); i++) {
        struct entity *entity = list_get(&this->_entities, i);
        entity_destroy(entity);
        free(entity);
    }
    list_destroy(&this->_entities);

    tmap_destroy(&this->_tmap);
}

const char* scene_get_name(const struct scene *this) {
    return this->_name;
}
void scene_set_name(struct scene *this, const char *name) {
    logger_info("Scene %s is renaming to %s", this->_name, name);
    this->_name = name;
}

static void handle_new_component(void *base, void *component) {
    struct scene *this = (struct scene *)base;
    tmap_register_component(&this->_tmap, component);
}

const struct tmap* scene_get_tmap(const struct scene *this) {
    return &this->_tmap;
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    logger_debug("Scene %s is capturing entity %s", this->_name, entity_get_name(entity));

    list_add(&this->_entities, entity);

    entity_set_parent(entity, this);

    for (int i = 0; i < entity_get_components_count(entity); i++) {
        struct component *component = entity_get_component_by_index(entity, i);
        handle_new_component(this, component);
    }

    action_add(&entity->on_component_captured, this, handle_new_component);
}

void scene_update(const struct scene *this, const struct update_context *context) {
    for (int i = 0; i < list_count(&this->_entities); i++) {
        const struct entity *entity = list_get(&this->_entities, i);
        entity_update(entity, context);
    }
}