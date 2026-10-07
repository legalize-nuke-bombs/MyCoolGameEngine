#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

#include <stdbool.h>

#include "components/component.h"
#include "../utils/rect.h"
#include "../api.h"
#include "../utils/uint128_t.h"
#include "../utils/list.h"

struct entity {
    char *_name;
    uint128_t _id;
    bool _awake;
    bool _alive;

    struct list _entities;
    struct list _components;

    struct rect _local_rect;
    struct rect _rect;

    struct entity *_parent;
    bool _in_scene;
};

MCGE_API struct entity* entity_create(char *name, struct entity *parent);
MCGE_API void entity_awake(struct entity *this);
MCGE_API void entity_destroy(struct entity *this);
MCGE_API void entity_mark_destroyed(struct entity *this);
MCGE_API void entity_destroy_marked(struct entity *this);

MCGE_API const char *entity_get_name(const struct entity *this);

MCGE_API uint128_t entity_get_id(const struct entity *this);

MCGE_API bool entity_is_awake(const struct entity *this);
MCGE_API bool entity_is_alive(const struct entity *this);

MCGE_API void entity_set_parent(struct entity *this, struct entity *new_parent);
MCGE_API struct entity* entity_get_parent(const struct entity *this);

MCGE_API void entity_set_in_scene(struct entity *this, bool in_scene);
MCGE_API bool entity_is_in_scene(const struct entity *this);

MCGE_API struct rect entity_get_local_rect(const struct entity *this);
MCGE_API void entity_set_local_rect(struct entity *this, struct rect new_local_rect);
MCGE_API struct rect entity_get_rect(const struct entity *this);

MCGE_API void entity_capture_entity(struct entity *this, struct entity *entity);
MCGE_API void entity_capture_component(struct entity *this, struct component *component);
MCGE_API void entity_recapture(struct entity *this);

enum entity_query {
    entity_query_local,
    entity_query_recursive
};
MCGE_API struct component* entity_try_get_component(const struct entity *this, const char *name, enum entity_query query);
MCGE_API struct component* entity_get_component(const struct entity *this, const char *name, enum entity_query query);

#endif
