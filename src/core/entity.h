#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include "components/component.h"

struct entity {
    const char *_name;

    struct component **_components;
    int _components_capacity;
    int _components_count;
};


void entity_init(struct entity *this);
void entity_destroy(struct entity *this);

const char *entity_get_name(const struct entity *this);
void entity_set_name(struct entity *this, const char *name);

int entity_get_components_count(const struct entity *this);
void entity_add_component(struct entity *this, struct component *component);

void entity_print(const struct entity *this);


#endif
