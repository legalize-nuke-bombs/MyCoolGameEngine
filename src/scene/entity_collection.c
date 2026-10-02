//
// Created by nikita on 23.09.2026.
//

#include "entity_collection.h"

#include <stdlib.h>

#include "entity.h"
#include "../logging/logger.h"
#include "../utils/list.h"
#include "../utils/pointer_dictionary.h"

struct entity_collection {
    struct dictionary *entities;
    struct list dead;
};

struct entity_collection *entity_collection_create(void) {
    logger_info("Entity_collection is creating...");
    struct entity_collection *this = calloc(1, sizeof(struct entity_collection));
    this->entities = pointer_dictionary_build(4);
    this->dead = list_create(16);
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

void entity_collection_add(struct entity_collection *this, struct entity *entity) {
    if (!entity_is_alive(entity)) {
        list_add(&this->dead, entity);
        return;
    }
    dictionary_try_add(this->entities, entity, entity);
}
void entity_collection_move_to_dead(struct entity_collection *this, struct entity *entity) {
    if (dictionary_remove(this->entities, entity)) {
        list_add(&this->dead, entity);
    }
}

void entity_collection_post_update(struct entity_collection *this) {
    for (int i = 0; i < list_count(&this->dead); i++) {
        entity_destroy(list_get(&this->dead, i));
    }
    list_clear(&this->dead);
}
