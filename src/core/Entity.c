#include "Entity.h"

#include <stdio.h>

void init_entity(struct Entity *entity, const char *name) {
    entity->name = name;
}

const char* get_entity_name(const struct Entity *entity) {
    return entity->name;
}
void set_entity_name(struct Entity *entity, const char *name) {
    entity->name = name;
}

void print_entity(const struct Entity *entity) {
    printf("Entity %s\n", entity->name);
}
