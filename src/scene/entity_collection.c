//
// Created by nikita on 23.09.2026.
//

#include "entity_collection.h"

#include <stdlib.h>

#include "entity.h"
#include "../logging/logger.h"
#include "../utils/action.h"
#include "../utils/list.h"
#include "../utils/pointer_dictionary.h"
#include "scene.h"

struct entity_collection {
    struct dictionary *entities;
    struct list dead;

    struct action* on_entity_captured;
    unsigned int on_entity_captured_token;

    struct action* on_entity_marked_destroyed;
    unsigned int on_entity_marked_destroyed_token;
};

static void handle_entity_captured(void *listener, void *context);
static void handle_entity_marked_destroyed(void *listener, void *context);

struct entity_collection *entity_collection_create(struct scene *scene) {
    logger_info("Entity_collection is creating...");
    struct entity_collection *this = calloc(1, sizeof(struct entity_collection));
    this->entities = pointer_dictionary_build(4);
    this->dead = list_create(16);

    this->on_entity_captured = scene_get_on_entity_captured(scene);
    action_subscribe(this->on_entity_captured, this, handle_entity_captured, &this->on_entity_captured_token);

    this->on_entity_marked_destroyed = scene_get_on_entity_marked_destroyed(scene);
    action_subscribe(this->on_entity_marked_destroyed, this, handle_entity_marked_destroyed, &this->on_entity_marked_destroyed_token);

    return this;
}

static void entity_collection_destroy_everyone(const struct entity_collection *this) {
    struct dictionary_iterator iterator = dictionary_begin(this->entities);
    struct dictionary_node node;
    while (dictionary_next(this->entities, &iterator, &node)) {
        entity_destroy(node.value);
    }
    for (int i = 0; i < list_count(&this->dead); i++) {
        entity_destroy(list_get(&this->dead, i));
    }
}

void entity_collection_destroy(struct entity_collection *this) {
    logger_info("Entity_collection is destroying...");
    action_unsubscribe(this->on_entity_marked_destroyed, this->on_entity_marked_destroyed_token);
    action_unsubscribe(this->on_entity_captured, this->on_entity_captured_token);
    entity_collection_destroy_everyone(this);
    dictionary_destroy(this->entities);
    list_destroy(&this->dead);
    free(this);
}

void entity_collection_awake_everyone(const struct entity_collection *this) {
    struct dictionary_iterator iterator = dictionary_begin(this->entities);
    struct dictionary_node node;
    while (dictionary_next(this->entities, &iterator, &node)) {
        entity_awake(node.value);
    }
}
void entity_collection_clear(struct entity_collection *this) {
    entity_collection_destroy_everyone(this);
    dictionary_clear(this->entities);
    list_clear(&this->dead);
}

static void handle_entity_captured(void *listener, void *context) {
    struct entity_collection *this = listener;
    struct entity *entity = context;
    if (!entity_is_alive(entity)) {
        list_add(&this->dead, entity);
        return;
    }
    dictionary_try_add(this->entities, entity, entity);

}
static void handle_entity_marked_destroyed(void *listener, void *context) {
    struct entity_collection *this = listener;
    struct entity *entity = context;
    dictionary_remove(this->entities, entity);
    list_add(&this->dead, entity);
}

void entity_collection_post_update(struct entity_collection *this) {
    for (int i = 0; i < list_count(&this->dead); i++) {
        entity_destroy(list_get(&this->dead, i));
    }
    list_clear(&this->dead);
}
