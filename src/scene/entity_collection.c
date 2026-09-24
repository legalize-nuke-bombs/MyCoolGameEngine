//
// Created by nikita on 23.09.2026.
//

#include "entity_collection.h"

#include <stdlib.h>

#include "entity.h"
#include "../logging/logger.h"
#include "../utils/list.h"

struct entity_collection {
    struct list *list;
};

struct entity_collection *entity_collection_create(void) {
    logger_info("Entity_collection is creating...");
    struct entity_collection *entities = malloc(sizeof(struct entity_collection));
    entities->list = list_create(16);
    return entities;
}
void entity_collection_destroy(struct entity_collection *this) {
    logger_info("Entity_collection is destroying...");
    for (int i = 0; i < list_count(this->list); i++) {
        struct entity *entity = list_get(this->list, i);
        entity_destroy(entity);
    }
    list_destroy(this->list);
    free(this);
}

void entity_collection_awake_everyone(const struct entity_collection *this) {
    for (int i = 0; i < list_count(this->list); i++) {
        struct entity *entity = list_get(this->list, i);
        entity_awake(entity);
    }
}

void entity_collection_add(const struct entity_collection *this, struct entity *entity) {
    list_add(this->list, entity);
}

void entity_collection_update(const struct entity_collection *this, const struct update_context *context) {
    for (int i = 0; i < list_count(this->list); i++) {
        const struct entity *entity = list_get(this->list, i);
        entity_update(entity, context);
    }
}

void entity_collection_destroy_dead(const struct entity_collection *this) {
    int ctr = 0;
    for (int i = 0; i < list_count(this->list); i++) {
        struct entity *entity = list_get(this->list, i);
        if (!entity_is_alive(entity)) {
            entity_destroy(entity);
            list_set(this->list, i, NULL);
            ctr++;
        }
    }
    list_remove_nulls(this->list);
    logger_debug("Entity_collection destroyed %d dead entities", ctr);
}