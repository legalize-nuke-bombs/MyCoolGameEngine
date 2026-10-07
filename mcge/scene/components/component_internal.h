#ifndef MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
#define MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>
#include "component.h"
#include "../../api.h"

struct fields;


struct component_vtable {
    const char* (*component_key)(void);
    size_t size;
    void (*on_create)(struct component *this, struct fields *fields);

    void (*on_awake)(struct component *this);
    void (*on_update)(struct component *this, const struct update_context *context);
    void (*on_visible_chunk_update)(struct component *this, const struct update_context *context);
    void (*on_simulation_chunk_update)(struct component *this, const struct update_context *context);
    bool (*is_chunkable)();

    void (*on_movement)(struct component *this);

    void (*on_disable)(struct component *this);
    void (*on_destroy)(struct component *this);
};

struct component {
    const struct component_vtable *vtable;
    uint128_t id;
    bool awake;
    bool alive;

    unsigned int last_visible_chunk_update_frame_number;
    unsigned int last_simulation_chunk_update_frame_number;

    struct entity *parent;
};

MCGE_API void component_base_create(struct component *this, const struct component_vtable *vtable, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_INTERNAL_H
