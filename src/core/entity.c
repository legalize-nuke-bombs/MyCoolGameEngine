#include "entity.h"

#include "../logging/logger.h"
#include <stdio.h>

void entity_init(struct entity *entity) {
    entity->_name = "Default entity";
}

const char* entity_get_name(const struct entity *entity) {
    return entity->_name;
}
void entity_set_name(struct entity *entity, const char *name) {
    if (entity->_name == NULL) {
        logger_warn("attempt to set null name to an entity");
        return;
    }
    entity->_name = name;
}

void entity_print(const struct entity *entity) {
    printf("Entity %s\n", entity->_name);
}
