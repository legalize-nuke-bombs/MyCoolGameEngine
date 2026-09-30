#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include <stdbool.h>

#include "../engine/update_context.h"
#include "components/component.h"

struct entity;
struct scene;
struct engine;
struct transform;

struct entity* entity_create(char *name, struct entity *parent, struct scene *scene);
struct entity* entity_clone(const struct entity* entity);
void entity_awake(struct entity *this);
void entity_destroy(struct entity *this);
void entity_mark_destroyed(struct entity *this);

const char *entity_get_name(const struct entity *this);

bool entity_is_awake(const struct entity *this);
bool entity_is_alive(const struct entity *this);

void entity_set_parent(struct entity *this, struct entity *new_parent);
struct entity* entity_get_parent(const struct entity *this);

void entity_set_scene(struct entity *this, struct scene *new_scene);
struct scene* entity_get_scene(const struct entity *this);

struct transform* entity_get_transform(const struct entity *this);

struct action* entity_get_action_on_component_captured(const struct entity *this);
struct action* entity_get_action_on_marked_destroyed(const struct entity *this);

void entity_capture_entity(struct entity *this, struct entity *entity);
void entity_capture_component(struct entity *this, struct component *component);
void entity_recapture_components(const struct entity *this);

struct component* entity_try_get_component(const struct entity *this, const char *name);
struct component* entity_get_component(const struct entity *this, const char *name);

#endif
