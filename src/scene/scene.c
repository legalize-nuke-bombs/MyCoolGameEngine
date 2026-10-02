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
#include "../subsystems/subsystem_collection.h"
#include "../interpreter/interpreter.h"


struct scene {
    struct subsystem base;

    char* name;
    bool awoken;

    struct action on_component_captured;
    struct action on_component_marked_destroyed;
    struct action on_entity_captured;
    struct action on_entity_marked_destroyed;
    struct action on_component_resize;

    char* switch_flag;

    struct entity_collection* entities;
    struct tmap *tmap;
    struct component_fabric* component_fabric;
    struct chunks* chunks;

    struct action* on_physics;
    unsigned int on_physics_subscription_token;
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

    this->on_component_captured = action_create();
    this->on_component_marked_destroyed = action_create();
    this->on_entity_captured = action_create();
    this->on_entity_marked_destroyed = action_create();
    this->on_component_resize = action_create();

    this->entities = entity_collection_create(this);
    this->tmap = tmap_create(this);
    this->component_fabric = component_fabric_create();
    this->chunks = chunks_create(this);

    return base;
}

void scene_on_destroy(struct subsystem *base) {
    struct scene *this = (struct scene*)base;
    logger_info("Scene %s is destroying...", this->name);

    component_fabric_destroy(this->component_fabric);
    chunks_destroy(this->chunks);
    entity_collection_destroy(this->entities);
    tmap_destroy(this->tmap);
    if (this->switch_flag != NULL) free(this->switch_flag);
    action_destroy(&this->on_component_resize);
    action_destroy(&this->on_entity_marked_destroyed);
    action_destroy(&this->on_component_marked_destroyed);
    action_destroy(&this->on_component_captured);
    free(this->name);
}



static void scene_update(void *listener, void *context);

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
    free(this->switch_flag);
    this->switch_flag = strdup(script_path);
}




static void scene_update(void *listener, void *context) {
    struct scene* this = listener;
    const struct update_context* update_context = context;

    scene_handle_switch(this);
    tmap_update(this->tmap, update_context);
    entity_collection_post_update(this->entities);
}




const char* scene_get_name(const struct scene *this) {
    return this->name;
}


void scene_notify_component_captured(const struct scene *this, struct component *component) {
    return action_invoke(&this->on_component_captured, component);
}
void scene_notify_component_marked_destroyed(const struct scene *this, struct component *component) {
    return action_invoke(&this->on_component_marked_destroyed, component);
}
void scene_notify_entity_marked_destroyed(const struct scene *this, struct entity *entity) {
    return action_invoke(&this->on_entity_marked_destroyed, entity);
}
void scene_notify_component_resize(const struct scene *this, struct component_on_rect_changed_callback_data *data) {
    return action_invoke(&this->on_component_resize, data);
}
struct action* scene_get_on_component_captured(struct scene *this) {
    return &this->on_component_captured;
}
struct action* scene_get_on_component_marked_destroyed(struct scene *this) {
    return &this->on_component_marked_destroyed;
}
struct action* scene_get_on_entity_captured(struct scene *this) {
    return &this->on_entity_captured;
}
struct action* scene_get_on_entity_marked_destroyed(struct scene *this) {
    return &this->on_entity_marked_destroyed;
}
struct action* scene_get_on_component_resize(struct scene *this) {
    return &this->on_component_resize;
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
const struct subsystem_collection* scene_get_subsystems(const struct scene* this) {
    return subsystem_get_subsystems((const struct subsystem*)this);
}

void scene_capture_entity(struct scene *this, struct entity *entity) {
    entity_set_scene(entity, this);
    logger_debug("Scene %s is capturing entity %s", this->name, entity_get_name(entity));

    action_invoke(&this->on_entity_captured, entity);

    entity_recapture_components(entity);

    if (this->awoken) {
        entity_awake(entity);
    }
}
