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
#include "../msystems/msystem.h"
#include "chunks/chunks.h"
#include "components/component_factory.h"
#include "../interpreter/interpreter.h"


static struct {
    const char* name;

    struct action on_component_captured;
    struct action on_component_marked_destroyed;
    struct action on_component_resize;

    char* switch_flag;

    struct entity_collection* entities;
    struct tmap *tmap;
    struct chunks* chunks;

    struct action* on_physics;
    unsigned int on_physics_subscription_token;
} scene;

static void scene_on_create(void) {
    scene.name = "Default scene";

    scene.on_component_captured = action_create();
    scene.on_component_marked_destroyed = action_create();
    scene.on_component_resize = action_create();

    component_factory_create();

    scene.entities = entity_collection_create();
    scene.tmap = tmap_create();
    scene.chunks = chunks_create();
}

static void scene_on_destroy(void) {
    logger_info("Scene %s is destroying...", scene.name);

    chunks_destroy(scene.chunks);
    entity_collection_destroy(scene.entities);
    tmap_destroy(scene.tmap);
    if (scene.switch_flag != NULL) free(scene.switch_flag);
    scene.switch_flag = NULL;
    action_destroy(&scene.on_component_resize);
    action_destroy(&scene.on_component_marked_destroyed);
    action_destroy(&scene.on_component_captured);

    component_factory_destroy();
}



static void scene_update(void *listener, void *context);

static void scene_on_enable(struct engine_arguments args) {
    scene.on_physics = engine_events_on_physics();
    action_subscribe(scene.on_physics, NULL, scene_update, &scene.on_physics_subscription_token);
}

static void scene_on_disable(void) {
    action_unsubscribe(scene.on_physics, scene.on_physics_subscription_token);
    scene.on_physics = NULL;
    entity_collection_clear(scene.entities);
    tmap_clear(scene.tmap);
    chunks_clear(scene.chunks);
}

const struct msystem scene_msystem = {
    .name = "scene",
    .on_create = scene_on_create,
    .on_enable = scene_on_enable,
    .on_disable = scene_on_disable,
    .on_destroy = scene_on_destroy
};



static void scene_handle_switch(void) {
    if (scene.switch_flag == NULL) {
        return;
    }
    logger_info("Scene is switching...");
    chunks_clear(scene.chunks);
    entity_collection_clear(scene.entities);
    tmap_clear(scene.tmap);
    interpreter_eval(scene.switch_flag);
    if (scene.switch_flag != NULL) free(scene.switch_flag);
    scene.switch_flag = NULL;
}

void scene_mark_switch(const char* script_path) {
    free(scene.switch_flag);
    scene.switch_flag = strdup(script_path);
}




static void scene_update(void *listener, void *context) {
    const struct update_context* update_context = context;

    scene_handle_switch();
    entity_collection_pre_update(scene.entities);
    tmap_update(scene.tmap, update_context);
}




const char* scene_get_name(void) {
    return scene.name;
}


void scene_notify_component_captured(struct component *component) {
    return action_invoke(&scene.on_component_captured, component);
}
void scene_notify_component_marked_destroyed(struct component *component) {
    return action_invoke(&scene.on_component_marked_destroyed, component);
}
void scene_notify_entity_marked_destroyed(struct entity *entity) {
    entity_collection_move_to_dead(scene.entities, entity);
}
void scene_notify_component_resize(struct component_on_rect_changed_callback_data *data) {
    return action_invoke(&scene.on_component_resize, data);
}
struct action* scene_get_on_component_captured(void) {
    return &scene.on_component_captured;
}
struct action* scene_get_on_component_marked_destroyed(void) {
    return &scene.on_component_marked_destroyed;
}
struct action* scene_get_on_component_resize(void) {
    return &scene.on_component_resize;
}


const struct tmap* scene_get_tmap(void) {
    return scene.tmap;
}
const struct chunks* scene_get_chunks(void) {
    return scene.chunks;
}

void scene_capture_entity(struct entity *entity) {
    entity_set_in_scene(entity, true);
    logger_debug("Scene %s is capturing entity %s", scene.name, entity_get_name(entity));

    entity_collection_add(scene.entities, entity);

    entity_recapture_components(entity);

    entity_awake(entity);
}
