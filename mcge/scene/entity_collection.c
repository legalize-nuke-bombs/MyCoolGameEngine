//
// Created by nikita on 23.09.2026.
//

#include "entity_collection.h"

#include <stdlib.h>

#include "entity.h"
#include "../logging/logger.h"
#include "../utils/list.h"
#include "../utils/pointer_dictionary.h"
#include "mcge/allocator/allocator.h"

struct entity_collection {
    struct dictionary *entities;
    struct list dead;
    struct dictionary_iterator gc_iterator;
};

#define ENTITY_COLLECTION_GC_ROOTS_PER_FRAME 100

struct entity_collection *entity_collection_create(void) {
    logger_info("Entity_collection is creating...");
    struct entity_collection *this = allocator_new0(1, sizeof(struct entity_collection));
    this->entities = pointer_dictionary_build(4);
    this->dead = list_create(16);
    return this;
}

static void entity_collection_destroy_marked_dead(struct entity_collection *this) {
    for (int i = 0; i < list_count(&this->dead); i++) {
        entity_destroy(list_get(&this->dead, i));
    }
    list_clear(&this->dead);
}
static void entity_collection_destroy_everyone(struct entity_collection *this) {
    struct dictionary_iterator iterator = dictionary_begin(this->entities);
    struct dictionary_node node;
    while (dictionary_next(this->entities, &iterator, &node)) {
        entity_mark_destroyed(node.value);
    }
    entity_collection_destroy_marked_dead(this);
}
static void entity_collection_destroy_exclude_resources(struct entity_collection *this) {
    struct dictionary_iterator iterator = dictionary_begin(this->entities);
    struct dictionary_node node;
    while (dictionary_next(this->entities, &iterator, &node)) {
        if (entity_is_resource(node.value)) {
            continue;
        }
        entity_mark_destroyed(node.value);
    }
    entity_collection_destroy_marked_dead(this);
}

void entity_collection_destroy(struct entity_collection *this) {
    logger_info("Entity_collection is destroying...");
    entity_collection_destroy_everyone(this);
    dictionary_destroy(this->entities);
    list_destroy(&this->dead);
    allocator_delete(this);
}

void entity_collection_clear_all(struct entity_collection *this) {
    entity_collection_destroy_everyone(this);
    this->gc_iterator = dictionary_begin(this->entities);
}
void entity_collection_clear_exclude_resources(struct entity_collection *this) {
    entity_collection_destroy_exclude_resources(this);
    this->gc_iterator = dictionary_begin(this->entities);
}

void entity_collection_capture(struct entity_collection *this, struct entity *entity) {
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

void entity_collection_pre_update(struct entity_collection *this) {
    entity_collection_destroy_marked_dead(this);
    for (int i = 0; i < ENTITY_COLLECTION_GC_ROOTS_PER_FRAME; i++) {
        struct dictionary_node node;
        if (!dictionary_next(this->entities, &this->gc_iterator, &node)) {
            this->gc_iterator = dictionary_begin(this->entities);
            break;
        }
        entity_collect_garbage(node.value);
    }
}

unsigned int entity_collection_count(const struct entity_collection *this) {
    return dictionary_count(this->entities);
}
