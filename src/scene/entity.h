#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include <stdbool.h>

#include "update_context.h"
#include "components/component.h"

struct entity;
struct scene;
struct transform;

struct entity* entity_create(char *name, struct scene *parent);
void entity_awake(struct entity *this);
void entity_destroy(struct entity *this);
void entity_mark_destroyed(struct entity *this);

const char *entity_get_name(const struct entity *this);

bool entity_is_awake(const struct entity *this);
bool entity_is_alive(const struct entity *this);

struct scene* entity_get_parent(const struct entity *this);

struct transform* entity_get_transform(const struct entity *this);

struct action* entity_get_action_on_component_captured(const struct entity *this);

int entity_get_components_count(const struct entity *this);
void entity_capture_component(struct entity *this, struct component *component);
struct component* entity_get_component_by_index(const struct entity *this, int index);

struct component* entity_try_get_component(const struct entity *this, const char *name);
struct component* entity_get_component(const struct entity *this, const char *name);

void entity_update(const struct entity *this, const struct update_context *context);

#endif
