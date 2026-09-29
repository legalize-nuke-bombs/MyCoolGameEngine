#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

#include <stdbool.h>
#include "../../engine/update_context.h"
#include "../../utils/rect.h"

struct component;
struct entity;

struct component* component_clone(const struct component *component);
void component_awake(struct component *this);
void component_destroy(struct component *this);
void component_mark_destroyed(struct component *this);

bool component_is_awake(const struct component *this);
bool component_is_alive(const struct component *this);

void component_set_parent(struct component *this, struct entity *parent);
struct entity* component_get_parent(const struct component *this);
const char* component_get_parent_name(const struct component *this);

struct rect component_get_local_rect(const struct component *this);
void component_set_local_rect(struct component *this, struct rect rect);

struct rect component_get_rect(const struct component *this);

const char* component_get_key(const struct component *this);

void component_update(struct component *this, const struct update_context *context);
bool component_is_updateable(const struct component *this);

#endif
