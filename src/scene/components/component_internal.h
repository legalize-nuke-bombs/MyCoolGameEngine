#ifndef MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
#define MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H

#include <stdbool.h>
#include "../../utils/vector2.h"
#include "component.h"


struct parser;

struct component_vtable {
    const char* (*component_key)(void);
    struct component* (*on_clone)(struct component base, const struct component *component);
    void (*on_awake)(struct component *this);
    void (*on_update)(struct component *this, const struct update_context *context);
    bool (*is_chunkable)();
    void (*on_disable)(struct component *this);
    void (*on_destroy)(struct component *this);
};

struct component {
    const struct component_vtable *vtable;
    bool awake;
    bool alive;
    struct rect local_rect;

    struct action* on_rect_changed;
    struct action* on_marked_destroyed;
    struct action* transform_on_rect_changed;
    unsigned int transform_on_rect_changed_subscription_token;

    struct entity *parent;
};

void component_base_create(struct component *this, const struct component_vtable *vtable, struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
