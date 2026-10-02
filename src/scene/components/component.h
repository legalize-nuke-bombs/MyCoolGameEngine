#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

#include <stdbool.h>
#include "../../engine/update_context.h"
#include "../../utils/rect.h"
#include "../../utils/rect_pair.h"

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
struct entity* component_get_global_parent(const struct component *this);
const char* component_get_global_parent_name(const struct component *this);
struct scene* component_get_scene(const struct component *this);

struct component_on_rect_changed_callback_data {
    struct component *component;
    struct rect_pair rect_pair;
};
struct rect component_get_rect(const struct component *this);
void component_notify_rect_changed(struct component *this, struct rect_pair rect_pair);

const char* component_get_key(const struct component *this);

void component_update(struct component *this, const struct update_context *context);
bool component_is_updateable(const struct component *this);

void component_visible_chunk_update(struct component *this, const struct update_context *context);
bool component_is_visible_chunkable(const struct component *this);
void component_simulation_chunk_update(struct component *this, const struct update_context *context);
bool component_is_simulation_chunkable(const struct component *this);
bool component_is_chunkable(const struct component *this);

#endif
