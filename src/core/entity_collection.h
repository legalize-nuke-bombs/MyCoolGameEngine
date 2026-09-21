//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
#define MYCOOLGAMEENGINE_ENTITY_COLLECTION_H

struct entity_collection {
    struct entity** _entities;
    int _entities_capacity;
    int _entities_count;
};

void entity_collection_init(struct entity_collection *this);
void entity_collection_destroy(struct entity_collection *this);

int entity_collection_get_entities_count(const struct entity_collection *this);

void entity_collection_print(const struct entity_collection *this);

#endif //MYCOOLGAMEENGINE_ENTITY_COLLECTION_H
