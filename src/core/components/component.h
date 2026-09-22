#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

#include "../update_context.h"
#include "../../utils/vector2.h"

struct component;

struct component_vtable {
    const char* (*component_key)(void);
    void (*on_awake)(struct component *this);
    void (*on_update)(struct component *this, const struct update_context *context);
    void (*on_destroy)(struct component *this);
};

struct component {
    struct component_vtable *_vtable;
    struct entity *_parent;
    struct vector2 local_position;
    struct vector2 local_scale;
};

void component_create(struct component *this);
void component_awake(struct component *this);
void component_destroy(struct component *this);

struct entity* component_get_parent(const struct component *this);
void component_set_parent(struct component *this, struct entity *parent);
const char* component_get_parent_name(const struct component *this);
struct vector2 component_get_position(const struct component *this);
struct vector2 component_get_scale(const struct component *this);

const char* component_get_key(const struct component *this);

void component_update(struct component *this, const struct update_context *context);

#endif
