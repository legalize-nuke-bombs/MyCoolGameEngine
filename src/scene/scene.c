//
// Created by nikita on 22.09.2026.
//

#include "scene.h"
#include "../logging/logger.h"

#include <stdlib.h>
#include <string.h>

#include "entity.h"
#include "entity_collection.h"
#include "../engine/events/engine_events.h"
#include "../utils/action.h"
#include "../subsystems/subsystem_internal.h"
#include "chunks/chunks.h"
#include "components/component_fabric.h"
#include "prefabs/prefab_manager.h"
#include "../subsystems/subsystem_collection.h"
#include "../interpreter/interpreter.h"


struct scene {
    struct subsystem base;

    char* name;
    bool awoken;

    char* switch_flag;

    struct entity_collection* entities;
    struct tmap *tmap;

    struct action* on_physics;
    unsigned int on_physics_subscription_token;

    struct component_fabric* component_fabric;
    struct chunks* chunks;
    struct prefab_manager* prefab_manager;
};

static const char* scene_get_subsystem_key() {
    return "scene";
}

static void scene_on_destroy(struct subsystem *base);
static void scene_on_enable(struct subsystem *base, struct engine_arguments args);
static void scene_on_disable(struct subsystem *base);

static struct subsystem_vtable scene_vtable = {
    .name = scene_get_subsystem_key,
    .on_destroy = scene_on_destroy,
    .on_enable = scene_on_enable,
    .on_disable = scene_on_disable
};

struct subsystem* scene_create(char *name, const struct subsystem_collection *subsystems) {
    struct scene *this = calloc(1, sizeof(struct scene));
    struct subsystem *base = (struct subsystem*)this;
    subsystem_create(base, &scene_vtable, subsystems);

    this->name = name;
    this->entities = entity_collection_create();
    this->tmap = tmap_create();
    this->component_fabric = component_fabric_create();
    this->chunks = chunks_create();
    this->prefab_manager = prefab_manager_create();

    return base;
}

void scene_on_destroy(struct subsystem *base) {
    const struct scene *this = (struct scene*)base;
    logger_info("Scene %s is destroying...", this->name);

    prefab_manager_destroy(this->prefab_manager);
    component_fabric_destroy(this->component_fabric);
    chunks_destroy(this->chunks);
    entity_collection_destroy(this->entities);
    tmap_destroy(this->tmap);
    if (this->switch_flag != NULL) free(this->switch_flag);
    free(this->name);
}


static void scene_handle_switch(struct scene *this) {
    if (this->switch_flag == NULL) {
        return;
    }
    logger_info("Scene is switching...");
    chunks_clear(this->chunks);
    entity_collection_clear(this->entities);
    tmap_clear(this->tmap);
    const struct interpreter* interpreter = (struct interpreter*)subsystem_collection_get(scene_get_subsystems(this), "interpreter");
    interpreter_eval(interpreter, this->switch_flag);
    if (this->switch_flag != NULL) free(this->switch_flag);
    this->switch_flag = NULL;
}

void scene_mark_switch(struct scene *this, const char* script_path) {
    this->switch_flag = strdup(script_path);
}




static void scene_run_gc(const struct scene *this) {
    const int destroyed = entity_collection_destroy_dead(this->entities);
    if (destroyed > 0) {
        logger_debug("Scene %s gc destroyed %d entities", this->name, destroyed);
    }
}

static void scene_update(void *listener, void *context) {
    struct scene* this = listener;
    const struct update_context* update_context = context;
    scene_handle_switch(this);
    tmap_update(this->tmap, update_context);
    scene_run_gc(this);
}



void scene_on_enable(struct subsystem *base, struct engine_arguments args) {
    struct scene *this = (struct scene*)base;
    this->awoken = true;
    entity_collection_awake_everyone(this->entities);
    this->on_physics = engine_events_on_physics((struct engine_events*)subsystem_get_subsystem(base, "engine_events"));
    action_subscribe(this->on_physics, this, scene_update, &this->on_physics_subscription_token);
}

void scene_on_disable(struct subsystem *base) {
    struct scene *this = (struct scene*)base;
    this->awoken = false;
    action_unsubscribe(this->on_physics, this->on_physics_subscription_token);
    this->on_physics = NULL;
    entity_collection_clear(this->entities);
    tmap_clear(this->tmap);
    chunks_clear(this->chunks);
    prefab_manager_clear(this->prefab_manager);
}

const char* scene_get_name(const struct scene *this) {
    return this->name;
}

static void handle_new_component(void *base, void *component) {
    const struct scene *this = base;
    tmap_register_component(this->tmap, component);
    chunks_register_component(this->chunks, component);
}

const struct tmap* scene_get_tmap(const struct scene *this) {
    return this->tmap;
}
const struct component_fabric* scene_get_component_fabric(const struct scene* this) {
    return this->component_fabric;
}
const struct chunks* scene_get_chunks(const struct scene *this) {
    return this->chunks;
}
const struct prefab_manager* scene_get_prefab_manager(const struct scene *this) {
    return this->prefab_manager;
}
const struct subsystem_collection* scene_get_subsystems(const struct scene* this) {
    return subsystem_get_subsystems((const struct subsystem*)this);
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    entity_set_scene(entity, this);
    logger_debug("Scene %s is capturing entity %s", this->name, entity_get_name(entity));

    if (this->awoken) {
        entity_awake(entity);
    }

    entity_collection_add(this->entities, entity);

    struct action* entity_on_component_captured = entity_get_action_on_component_captured(entity);
    unsigned int subscription_token; // We do not unsubscribe because scene always lives longer than it's entities
    action_subscribe(entity_on_component_captured, this, handle_new_component, &subscription_token);

    entity_recapture_components(entity);
}
