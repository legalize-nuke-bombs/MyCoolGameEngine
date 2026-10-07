//
// Created by nikita on 23.09.2026.
//

#include "entity_collection.h"

#include <stdlib.h>

#include "entity.h"
#include "../logging/logger.h"
#include "../utils/list.h"
#include "utils/entity_dictionary.h"

struct entity_collection {
    struct dictionary *entities;
    struct list dead;
};

struct entity_collection *entity_collection_create(void) {
    logger_info("Entity_collection is creating...");
    struct entity_collection *this = calloc(1, sizeof(struct entity_collection));
    this->entities = entity_dictionary_build(10);
    this->dead = list_create(16);
    return this;
}

static void entity_collection_destroy_everyone(const struct entity_collection *this) {
    // Everyone is disabled before anyone is freed, so on_disable can still reach other components
    struct dictionary_iterator iterator = dictionary_begin(this->entities);
    struct dictionary_node node;
    while (dictionary_next(this->entities, &iterator, &node)) {
        entity_mark_destroyed(node.value);
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

void entity_collection_clear(struct entity_collection *this) {
    entity_collection_destroy_everyone(this);
    dictionary_clear(this->entities);
    list_clear(&this->dead);
}

struct entity* entity_collection_try_get(const struct entity_collection *this, uint128_t id) {
    return dictionary_get(this->entities, &id);
}
void entity_collection_capture(struct entity_collection *this, struct entity *entity) {
    if (!entity_is_alive(entity)) {
        list_add(&this->dead, entity);
        return;
    }
    dictionary_try_add(this->entities, &entity->_id, entity);
}
void entity_collection_move_to_dead(struct entity_collection *this, struct entity *entity) {
    if (dictionary_remove(this->entities, &entity->_id)) {
        list_add(&this->dead, entity);
    }
}

void entity_collection_pre_update(struct entity_collection *this) {
    for (int i = 0; i < list_count(&this->dead); i++) {
        entity_destroy(list_get(&this->dead, i));
    }
    list_clear(&this->dead);
}
