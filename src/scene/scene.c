//
// Created by nikita on 22.09.2026.
//

#include "scene.h"
#include "../logging/logger.h"

#include <stdlib.h>

#include "entity.h"
#include "entity_collection.h"
#include "../engine/engine.h"
#include "../engine/engine_events.h"
#include "../utils/action.h"


#define GC_INTERVAL 5


struct scene {
    char* name;
    struct entity_collection* entities;
    struct tmap *tmap;
    double gcTimer;

    struct action* on_physics;
    unsigned int on_physics_subscription_token;
    const struct engine *engine;
};

struct scene* scene_create(char *name, const struct engine* engine) {
    struct scene *this = malloc(sizeof(struct scene));

    this->name = name;
    logger_info("Scene %s is initializing...", this->name);
    this->entities = entity_collection_create();
    this->tmap = tmap_create();

    this->on_physics = NULL;
    this->on_physics_subscription_token = 0;
    this->engine = engine;

    return this;
}

static void scene_run_gc(struct scene *this, double dt) {
    this->gcTimer += dt;
    if (this->gcTimer < GC_INTERVAL) {
        return;
    }
    this->gcTimer -= GC_INTERVAL;
    logger_debug("Scene %s launched gc", this->name);
    const int tmap_gc_num = tmap_remove_dead(this->tmap);
    const int entity_collection_gc_num = entity_collection_destroy_dead(this->entities);
    if (tmap_gc_num + entity_collection_gc_num > 0) {
        logger_info("Scene %s gc report: tmap cleared %d components, entity collection cleared %d entities", this->name, tmap_gc_num, entity_collection_gc_num);
    }
}

static void scene_update(void *listener, void *context) {
    struct scene* this = listener;
    const struct update_context* update_context = context;
    entity_collection_update(this->entities, update_context);
    scene_run_gc(this, update_context->dt);
}

void scene_awake(struct scene *this) {
    logger_info("Scene %s is awaking...", this->name);
    entity_collection_awake_everyone(this->entities);
    this->gcTimer = 0;
    this->on_physics = engine_events_on_physics(engine_get_events(this->engine));
    action_subscribe(this->on_physics, this, scene_update, &this->on_physics_subscription_token);
}
void scene_destroy(struct scene *this) {
    logger_info("Scene %s is destroying...", this->name);

    action_unsubscribe(this->on_physics, this->on_physics_subscription_token);
    entity_collection_destroy(this->entities);
    tmap_destroy(this->tmap);
    free(this->name);
    free(this);
}

const char* scene_get_name(const struct scene *this) {
    return this->name;
}

static void handle_new_component(void *base, void *component) {
    const struct scene *this = base;
    tmap_register_component(this->tmap, component);
}

const struct tmap* scene_get_tmap(const struct scene *this) {
    return this->tmap;
}
const struct engine *scene_get_engine(const struct scene *this) {
    return this->engine;
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

    const struct action* entity_on_component_captured = entity_get_action_on_component_captured(entity);
    unsigned int subscription_token; // We do not unsubscribe because scene always lives longer than it's entities
    action_subscribe(entity_on_component_captured, this, handle_new_component, &subscription_token);
}
