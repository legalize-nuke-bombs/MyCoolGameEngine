#include "entity.h"

#include <stdio.h>
#include <stdlib.h>


#define DEFAULT_COMPONENTS_CAPACITY 1


void entity_init(struct entity *entity) {
    entity->_name = "Default entity";

    entity->_components = malloc(sizeof(struct component) * DEFAULT_COMPONENTS_CAPACITY);
    entity->_components_capacity = DEFAULT_COMPONENTS_CAPACITY;
    entity->_components_count = 0;
}
void entity_destroy(struct entity *entity) {
    free(entity->_components);
    entity->_components = NULL;
}

const char* entity_get_name(const struct entity *entity) {
    return entity->_name;
}
void entity_set_name(struct entity *entity, const char *name) {
    entity->_name = name;
}

int entity_get_components_count(const struct entity *entity) {
    return entity->_components_count;
}

void entity_print(const struct entity *entity) {
    printf("Entity %s (%d components count, %d components capacity)\n", entity->_name, entity->_components_count, entity->_components_capacity);
}
