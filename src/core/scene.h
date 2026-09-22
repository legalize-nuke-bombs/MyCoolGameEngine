//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "../utils/list.h"
#include "update_context.h"
#include "tmap.h"

struct scene {
    const char* _name;
    struct list _entities;
    struct tmap _tmap;
};

void scene_init(struct scene *this);
void scene_destroy(const struct scene *this);

const char* scene_get_name(const struct scene *this);
void scene_set_name(struct scene *this, const char *name);

void handle_new_component(void *base, void *component);

void scene_capture_entity(struct scene *this, struct entity *entity);

void scene_update(const struct scene *this, const struct update_context *context);

#endif //MYCOOLGAMEENGINE_SCENE_H
