//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "tmap/tmap.h"

struct entity;
struct msystem;

extern const struct msystem scene_msystem;

void scene_mark_switch(const char *script_path);

const char* scene_get_name(void);

void scene_notify_component_captured(struct component *component);
void scene_notify_component_marked_destroyed(struct component *component);
void scene_notify_entity_marked_destroyed(struct entity *entity);
void scene_notify_component_resize(struct component_on_rect_changed_callback_data *data);
struct action* scene_get_on_component_captured(void);
struct action* scene_get_on_component_marked_destroyed(void);
struct action* scene_get_on_component_resize(void);

const struct tmap* scene_get_tmap(void);
const struct chunks* scene_get_chunks(void);

void scene_capture_entity(struct entity *entity);

#endif //MYCOOLGAMEENGINE_SCENE_H
