#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

#include <stdbool.h>
#include "../../engine/update_context.h"
#include "../../utils/vector2.h"

struct component;
struct entity;

void component_awake(struct component *this);
void component_destroy(struct component *this);
void component_mark_destroyed(struct component *this);

bool component_is_awake(const struct component *this);
bool component_is_alive(const struct component *this);

struct entity* component_get_parent(const struct component *this);
const char* component_get_parent_name(const struct component *this);

struct vector2 component_get_local_position(const struct component *this);
struct vector2 component_get_local_scale(const struct component *this);

struct vector2 component_get_position(const struct component *this);
struct vector2 component_get_scale(const struct component *this);

const char* component_get_key(const struct component *this);

void component_update(struct component *this, const struct update_context *context);
bool component_is_updateable(const struct component *this);

#endif
