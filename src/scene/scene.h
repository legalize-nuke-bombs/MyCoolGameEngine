//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "tmap/tmap.h"

struct scene;
struct entity;
struct subsystem;
struct subsystem_collection;

struct subsystem* scene_create(char* name, const struct subsystem_collection *subsystems);

void scene_mark_switch(struct scene *this, const char *script_path);

const char* scene_get_name(const struct scene *this);

const struct tmap* scene_get_tmap(const struct scene *this);
const struct component_fabric* scene_get_component_fabric(const struct scene* this);
const struct chunks* scene_get_chunks(const struct scene *this);
const struct subsystem_collection* scene_get_subsystems(const struct scene* this);

void scene_capture_entity(struct scene *this, struct entity *entity);

#endif //MYCOOLGAMEENGINE_SCENE_H
