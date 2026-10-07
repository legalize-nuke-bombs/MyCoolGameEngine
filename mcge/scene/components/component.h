#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

#include <stdbool.h>
#include "../../engine/update_context.h"
#include "../../utils/rect.h"
#include "../../utils/rect_pair.h"
#include "../../utils/uint128_t.h"
#include "../../api.h"

struct component;
struct entity;

MCGE_API void component_awake(struct component *this);
MCGE_API void component_destroy(struct component *this);
MCGE_API void component_mark_destroyed(struct component *this);

MCGE_API uint128_t component_get_id(const struct component *this);

MCGE_API bool component_is_awake(const struct component *this);
MCGE_API bool component_is_alive(const struct component *this);

MCGE_API void component_set_parent(struct component *this, struct entity *parent);
MCGE_API struct entity* component_get_parent(const struct component *this);
MCGE_API const char* component_get_parent_name(const struct component *this);
MCGE_API struct entity* component_get_global_parent(const struct component *this);
MCGE_API const char* component_get_global_parent_name(const struct component *this);
MCGE_API bool component_is_in_scene(const struct component *this);

struct component_on_rect_changed_callback_data {
    struct component *component;
    struct rect_pair rect_pair;
};
MCGE_API struct rect component_get_rect(const struct component *this);
MCGE_API void component_notify_rect_changed(struct component *this, struct rect_pair rect_pair);

MCGE_API const char* component_get_key(const struct component *this);

MCGE_API void component_update(struct component *this, const struct update_context *context);
MCGE_API bool component_is_updateable(const struct component *this);

MCGE_API void component_visible_chunk_update(struct component *this, const struct update_context *context);
MCGE_API bool component_is_visible_chunkable(const struct component *this);
MCGE_API void component_simulation_chunk_update(struct component *this, const struct update_context *context);
MCGE_API bool component_is_simulation_chunkable(const struct component *this);
MCGE_API bool component_is_chunkable(const struct component *this);

#endif
