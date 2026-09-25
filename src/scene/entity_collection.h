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

void entity_collection_awake_everyone(const struct entity_collection *this);
void entity_collection_clear(const struct entity_collection *this);

void entity_collection_add(const struct entity_collection *this, struct entity *entity);

void entity_collection_update(const struct entity_collection *this, const struct update_context *context);

int entity_collection_destroy_dead(const struct entity_collection *this);

#endif //MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
