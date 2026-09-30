#ifndef MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
#define MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H

#include <stdbool.h>
#include "component.h"
#include "../../utils/action.h"


struct component_vtable {
    const char* (*component_key)(void);
    struct component* (*on_clone)(struct component base, const struct component *component);
    void (*on_awake)(struct component *this);
    void (*on_update)(struct component *this, const struct update_context *context);
    void (*on_visible_chunk_update)(struct component *this, const struct update_context *context);
    void (*on_simulation_chunk_update)(struct component *this, const struct update_context *context);
    bool (*is_chunkable)();
    void (*on_disable)(struct component *this);
    void (*on_destroy)(struct component *this);
};

struct component {
    const struct component_vtable *vtable;
    bool awake;
    bool alive;

    struct action on_rect_changed;
    struct action on_marked_destroyed;

    unsigned int last_visible_chunk_update_frame_number;
    unsigned int last_simulation_chunk_update_frame_number;

    struct entity *parent;
};

void component_base_create(struct component *this, const struct component_vtable *vtable, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
