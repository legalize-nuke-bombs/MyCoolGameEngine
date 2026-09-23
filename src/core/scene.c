//
// Created by nikita on 22.09.2026.
//

#include "scene.h"
#include "../logging/logger.h"

#include <stdlib.h>

#include "entity.h"
#include "../utils/list.h"


struct scene {
    const char* name;
    struct list *entities;
    struct tmap *tmap;
};

struct scene* scene_create(void) {
    struct scene *this = malloc(sizeof(struct scene));
    this->name = "Default scene";
    logger_info("Scene %s is initializing...", this->name);
    this->entities = list_create(16);
    this->tmap = tmap_create();
    return this;
}
void scene_awake(const struct scene *this) {
    logger_info("Scene %s is awaking...", this->name);
    for (int i = 0; i < list_count(this->entities); i++) {
        struct entity *entity = list_get(this->entities, i);
        entity_awake(entity);
    }
}
void scene_destroy(struct scene *this) {
    logger_info("Scene %s is destroying...", this->name);

    for (int i = 0; i < list_count(this->entities); i++) {
        struct entity *entity = list_get(this->entities, i);
        entity_destroy(entity);
    }
    list_destroy(this->entities);

    tmap_destroy(this->tmap);
    free(this);
}

const char* scene_get_name(const struct scene *this) {
    return this->name;
}
void scene_set_name(struct scene *this, const char *name) {
    logger_info("Scene %s is renaming to %s", this->name, name);
    this->name = name;
}

static void handle_new_component(void *base, void *component) {
    struct scene *this = (struct scene *)base;
    tmap_register_component(this->tmap, component);
}

const struct tmap* scene_get_tmap(const struct scene *this) {
    return this->tmap;
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    logger_debug("Scene %s is capturing entity %s", this->name, entity_get_name(entity));

    list_add(this->entities, entity);

    entity_set_parent(entity, this);

    for (int i = 0; i < entity_get_components_count(entity); i++) {
        struct component *component = entity_get_component_by_index(entity, i);
        handle_new_component(this, component);
    }

    entity_subscribe_on_component_captured(entity, this, handle_new_component);
}

void scene_update(const struct scene *this, const struct update_context *context) {
    for (int i = 0; i < list_count(this->entities); i++) {
        const struct entity *entity = list_get(this->entities, i);
        entity_update(entity, context);
    }
}