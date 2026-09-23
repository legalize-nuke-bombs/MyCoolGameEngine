//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "update_context.h"
#include "tmap.h"

struct scene;
struct entity;
struct engine;

struct scene* scene_create(const char* name, const struct engine *engine);
void scene_awake(struct scene *this);
void scene_destroy(struct scene *this);

const char* scene_get_name(const struct scene *this);

const struct tmap* scene_get_tmap(const struct scene *this);
const struct engine* scene_get_engine(const struct scene *this);

void scene_capture_entity(struct scene *this, struct entity *entity);

void scene_update(struct scene *this, const struct update_context *context);

#endif //MYCOOLGAMEENGINE_SCENE_H
