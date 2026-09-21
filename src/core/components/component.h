#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

#include "../update_context.h"

struct component;

struct component_vtable {
    void (*on_update)(struct component *self, const struct update_context *context);
    void (*on_destroy)(struct component *self);
};

struct component {
    struct component_vtable *_vtable;
    struct entity *_parent;
};

void component_init(struct component *this);
void component_destroy(struct component *this);

struct entity* component_get_parent(const struct component *this);
void component_set_parent(struct component *this, struct entity *parent);

void component_update(struct component *this, const struct update_context *context);

#endif
