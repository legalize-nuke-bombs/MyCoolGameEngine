//
// Created by nikita on 22.09.2026.
//

#include "entity_collection.h"

#include <stdio.h>
#include <stdlib.h>

#include "entity.h"


#define DEFAULT_CAPACITY 1


void entity_collection_init(struct entity_collection *this) {
    this->_entities = malloc(sizeof(struct entity*) * DEFAULT_CAPACITY);
    this->_entities_capacity = DEFAULT_CAPACITY;
    this->_entities_count = 0;
}
void entity_collection_destroy(struct entity_collection *this) {
    for (int i = 0; i < this->_entities_count; i++) {
        const struct entity *entity = this->_entities[i];
        entity_destroy(entity);
    }
    free(this->_entities);
}

int entity_collection_get_entities_count(const struct entity_collection *this) {
    return this->_entities_count;
}

void entity_collection_print(const struct entity_collection *this) {
    printf("Entity collection (%d entities count, %d entities capacity)\n", this->_entities_count, this->_entities_capacity);
}