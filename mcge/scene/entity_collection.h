//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
#define MYCOOLGAMEENGINE_ENTITY_COLLECTION_H

#include "../api.h"

struct entity;
struct entity_collection;
struct update_context;

MCGE_API struct entity_collection *entity_collection_create(void);
MCGE_API void entity_collection_destroy(struct entity_collection *this);

MCGE_API void entity_collection_clear(struct entity_collection *this);

MCGE_API void entity_collection_capture(struct entity_collection *this, struct entity *entity);
MCGE_API void entity_collection_move_to_dead(struct entity_collection *this, struct entity *entity);

MCGE_API void entity_collection_pre_update(struct entity_collection *this);

#endif //MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
