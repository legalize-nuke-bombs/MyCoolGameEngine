//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "entity_collection.h"

struct scene {
    const char* _name;
    struct entity_collection* _entity_collection;
};

void scene_init(struct scene *this);
void scene_destroy(const struct scene *this);

const char* scene_get_name(struct scene *this);
void scene_set_name(struct scene *this, const char *name);

struct entity_collection* scene_get_entity_collection(const struct scene *this);
void scene_set_entity_collection(struct scene *this, struct entity_collection *entity_collection);

void scene_update(struct scene *this, const struct update_context *context);

#endif //MYCOOLGAMEENGINE_SCENE_H
