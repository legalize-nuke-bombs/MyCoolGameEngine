//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "update_context.h"
#include "tmap.h"

struct scene;
struct entity;
struct engine_context;

struct scene* scene_create(struct engine_context *engine_context);
void scene_awake(const struct scene *this);
void scene_destroy(struct scene *this);

const char* scene_get_name(const struct scene *this);
void scene_set_name(struct scene *this, const char *name);

const struct tmap *scene_get_tmap(const struct scene *this);

void scene_capture_entity(struct scene *this, struct entity *entity);

void scene_update(const struct scene *this, const struct update_context *context);

#endif //MYCOOLGAMEENGINE_SCENE_H
