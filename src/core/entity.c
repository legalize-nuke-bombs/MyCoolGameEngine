#include "entity.h"

#include <stdio.h>

void init_entity(struct entity *entity) {
    entity->name = "Default entity";
}

const char* get_entity_name(const struct entity *entity) {
    return entity->name;
}
void set_entity_name(struct entity *entity, const char *name) {
    entity->name = name;
}

void print_entity(const struct entity *entity) {
    printf("Entity %s\n", entity->name);
}
