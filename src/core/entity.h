#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include "update_context.h"
#include "components/component.h"
#include "../utils/list.h"

struct entity {
    const char *_name;

    struct list _components;
};

void entity_init(struct entity *this);
void entity_destroy(const struct entity *this);

const char *entity_get_name(const struct entity *this);
void entity_set_name(struct entity *this, const char *name);

int entity_get_components_count(const struct entity *this);
void entity_capture_component(struct entity *this, struct component *component);
struct component* entity_get_component(const struct entity *this, int index);

void entity_update(const struct entity *this, const struct update_context *context);

#endif
