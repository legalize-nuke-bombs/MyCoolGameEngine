#ifndef MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
#define MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H

#include <stdbool.h>

#include "component.h"

struct component_vtable {
    const char* (*component_key)(void);
    void (*on_awake)(struct component *this);
    void (*on_update)(struct component *this, const struct update_context *context);
    void (*on_destroy)(struct component *this);
    void (*on_free)(struct component *this);
};

struct component {
    const struct component_vtable *vtable;
    bool awake;
    bool alive;
    struct vector2 local_position;
    struct vector2 local_scale;
    struct entity *parent;
};

void component_init(struct component *this, const struct component_vtable *vtable, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
