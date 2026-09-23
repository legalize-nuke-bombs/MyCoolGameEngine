//
// Created by nikita on 22.09.2026.
//

#include "scene.h"
#include "../logging/logger.h"

#include <stdlib.h>

#include "entity.h"
#include "entity_collection.h"


#define GC_INTERVAL 1


struct scene {
    const char* name;
    struct entity_collection* entities;
    struct tmap *tmap;
    double gcTimer;
    const struct engine_context *engine_context;
};

struct scene* scene_create(const struct engine_context* engine_context) {
    struct scene *this = malloc(sizeof(struct scene));
    this->name = "Default scene";
    logger_info("Scene %s is initializing...", this->name);
    this->entities = entity_collection_create();
    this->tmap = tmap_create();
    this->engine_context = engine_context;
    return this;
}
void scene_awake(struct scene *this) {
    logger_info("Scene %s is awaking...", this->name);
    entity_collection_awake_everyone(this->entities);
    this->gcTimer = 0;
}
void scene_destroy(struct scene *this) {
    logger_info("Scene %s is destroying...", this->name);

    entity_collection_destroy(this->entities);
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
    const struct scene *this = base;
    tmap_register_component(this->tmap, component);
}

const struct tmap* scene_get_tmap(const struct scene *this) {
    return this->tmap;
}
const struct engine_context *scene_get_engine_context(const struct scene *this) {
    return this->engine_context;
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    const struct scene *owner = entity_get_parent(entity);
    if (owner != this) {
        logger_error("Scene %s cannot capture entity %s owned by scene %s", this->name, entity_get_name(entity), owner != NULL ? owner->name : "<none>");
        return;
    }
    logger_debug("Scene %s is capturing entity %s", this->name, entity_get_name(entity));

    entity_collection_add(this->entities, entity);

    for (int i = 0; i < entity_get_components_count(entity); i++) {
        struct component *component = entity_get_component_by_index(entity, i);
        handle_new_component(this, component);
    }

    entity_subscribe_on_component_captured(entity, this, handle_new_component);
}

static void scene_run_gc(struct scene *this, double dt) {
    this->gcTimer += dt;
    if (this->gcTimer < GC_INTERVAL) {
        return;
    }
    this->gcTimer -= GC_INTERVAL;
    logger_info("Scene %s launched gc", this->name);
    tmap_remove_dead(this->tmap);
    entity_collection_destroy_dead(this->entities);
}

void scene_update(struct scene *this, const struct update_context *context) {
    entity_collection_update(this->entities, context);
    scene_run_gc(this, context->dt);
}
