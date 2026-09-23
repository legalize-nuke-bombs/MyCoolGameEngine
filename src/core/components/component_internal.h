#ifndef MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
#define MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H

#include "component.h"

struct component_vtable {
    const char* (*component_key)(void);
    void (*on_awake)(struct component *this);
    void (*on_update)(struct component *this, const struct update_context *context);
    void (*on_destroy)(struct component *this);
};

struct component {
    const struct component_vtable *vtable;
    struct entity *parent;
    struct vector2 local_position;
    struct vector2 local_scale;
};

void component_init(struct component *this, const struct component_vtable *vtable);

#endif //MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
