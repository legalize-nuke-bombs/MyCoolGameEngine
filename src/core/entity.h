#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include "update_context.h"
#include "components/component.h"
#include "../utils/list.h"
#include "../utils/action.h"

struct entity {
    const char *_name;
    struct list _components;
    struct action on_component_captured;
};

void entity_create(struct entity *this);
void entity_awake(const struct entity *this);
void entity_destroy(const struct entity *this);

const char *entity_get_name(const struct entity *this);
void entity_set_name(struct entity *this, const char *name);

int entity_get_components_count(const struct entity *this);
void entity_capture_component(struct entity *this, struct component *component);
struct component* entity_get_component_by_index(const struct entity *this, int index);

struct component* entity_try_get_component(const struct entity *this, const char *name);
struct component* entity_get_component(const struct entity *this, const char *name);

void entity_update(const struct entity *this, const struct update_context *context);

#endif
