//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "tmap.h"

struct scene;
struct entity;
struct engine;
struct subsystem_collection;

struct scene* scene_create(char* name, const struct engine *engine, const struct subsystem_collection *subsystems);
void scene_destroy(struct scene *this);

void scene_awake(struct scene *this);
void scene_disable(struct scene *this);

const char* scene_get_name(const struct scene *this);

const struct tmap* scene_get_tmap(const struct scene *this);
const struct engine* scene_get_engine(const struct scene *this);
const struct subsystem_collection* scene_get_subsystems(const struct scene* this);

void scene_capture_entity(struct scene *this, struct entity *entity);

#endif //MYCOOLGAMEENGINE_SCENE_H
