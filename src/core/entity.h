#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include "components/component.h"

struct entity {
    const char *_name;

    struct component *_components;
    int _components_capacity;
    int _components_count;
};


void entity_init(struct entity *entity);
void entity_destroy(struct entity *entity);

const char *entity_get_name(const struct entity *entity);
void entity_set_name(struct entity *entity, const char *name);

int entity_get_components_count(const struct entity *entity);

void entity_print(const struct entity *entity);


#endif
