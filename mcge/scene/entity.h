#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include <stdbool.h>

#include "components/component.h"
#include "../utils/rect.h"

struct entity;

struct entity* entity_create(char *name, struct entity *parent);
void entity_awake(struct entity *this);
void entity_destroy(struct entity *this);
void entity_mark_destroyed(struct entity *this);

const char *entity_get_name(const struct entity *this);

bool entity_is_awake(const struct entity *this);
bool entity_is_alive(const struct entity *this);

void entity_set_parent(struct entity *this, struct entity *new_parent);
struct entity* entity_get_parent(const struct entity *this);

void entity_set_in_scene(struct entity *this, bool in_scene);
bool entity_is_in_scene(const struct entity *this);

struct rect entity_get_local_rect(const struct entity *this);
void entity_set_local_rect(struct entity *this, struct rect new_local_rect);
struct rect entity_get_rect(const struct entity *this);

void entity_capture_entity(struct entity *this, struct entity *entity);
void entity_capture_component(struct entity *this, struct component *component);
void entity_recapture_components(const struct entity *this);

enum entity_query {
    entity_query_local,
    entity_query_recursive
};
struct component* entity_try_get_component(const struct entity *this, const char *name, enum entity_query query);
struct component* entity_get_component(const struct entity *this, const char *name, enum entity_query query);

#endif
