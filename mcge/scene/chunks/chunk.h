//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CHUNK_H
#define MYCOOLGAMEENGINE_CHUNK_H

#include "../../api.h"

struct component;
struct dictionary;

struct chunk {
    struct dictionary* types;
};

MCGE_API void chunk_destroy(struct chunk *this);
MCGE_API void chunk_clear(const struct chunk *this);

MCGE_API void chunk_try_add_component(struct chunk *this, const struct component* component);
MCGE_API void chunk_try_remove_component(const struct chunk *this, const struct component* component);

MCGE_API struct dictionary* chunk_get_types(const struct chunk* this);
MCGE_API struct dictionary* chunk_get_components_by_type(const struct chunk* this, const char* component_type);

#endif //MYCOOLGAMEENGINE_CHUNK_H
