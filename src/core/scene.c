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
    this->_name = name;
}

void handle_new_component(void *base, void *component) {
    struct scene *this = (struct scene *)base;
    tmap_register_component(&this->_tmap, component);
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    logger_debug("Scene %s is capturing entity %s", this->_name, entity_get_name(entity));

    list_add(&this->_entities, entity);

    for (int i = 0; i < entity_get_components_count(entity); i++) {
        struct component *component = entity_get_component(entity, i);
        handle_new_component(this, component);
    }

    struct action_method* action_method = malloc(sizeof(action_method));
    action_method->listener = this;
    action_method->action = handle_new_component;
    action_capture(&entity->on_component_captured, action_method);
}

void scene_update(const struct scene *this, const struct update_context *context) {
    for (int i = 0; i < list_count(&this->_entities); i++) {
        const struct entity *entity = list_get(&this->_entities, i);
        entity_update(entity, context);
    }
}