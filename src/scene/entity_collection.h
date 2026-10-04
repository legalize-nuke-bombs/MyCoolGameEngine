//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
#define MYCOOLGAMEENGINE_ENTITY_COLLECTION_H

struct entity;
struct entity_collection;
struct update_context;

struct entity_collection *entity_collection_create(void);
void entity_collection_destroy(struct entity_collection *this);

void entity_collection_clear(struct entity_collection *this);

void entity_collection_add(struct entity_collection *this, struct entity *entity);
void entity_collection_move_to_dead(struct entity_collection *this, struct entity *entity);

void entity_collection_pre_update(struct entity_collection *this);

#endif //MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
