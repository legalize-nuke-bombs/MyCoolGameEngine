//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
#define MYCOOLGAMEENGINE_ENTITY_COLLECTION_H

struct entity;
struct entity_collection;
struct update_context;
struct scene;

struct entity_collection *entity_collection_create(struct scene *scene);
void entity_collection_destroy(struct entity_collection *this);

void entity_collection_awake_everyone(const struct entity_collection *this);
void entity_collection_clear(struct entity_collection *this);

void entity_collection_post_update(struct entity_collection *this);

#endif //MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
