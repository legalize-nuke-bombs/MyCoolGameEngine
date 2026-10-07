//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_H
#define MYCOOLGAMEENGINE_SCENE_H

#include "tmap/tmap.h"
#include "../api.h"
#include "../utils/uint128_t.h"

struct entity;
struct msystem;

MCGE_API extern const struct msystem scene_msystem;

MCGE_API void scene_mark_switch(const char *script_path);

MCGE_API const char* scene_get_name(void);

MCGE_API void scene_notify_entity_captured(struct entity *entity);
MCGE_API void scene_notify_entity_marked_destroyed(struct entity *entity);
MCGE_API void scene_notify_component_captured(struct component *component);
MCGE_API void scene_notify_component_marked_destroyed(struct component *component);
MCGE_API void scene_notify_component_resize(struct component_on_rect_changed_callback_data *data);
MCGE_API struct action* scene_get_on_entity_captured(void);
MCGE_API struct action* scene_get_on_entity_marked_destroyed(void);
MCGE_API struct action* scene_get_on_component_captured(void);
MCGE_API struct action* scene_get_on_component_marked_destroyed(void);
MCGE_API struct action* scene_get_on_component_resize(void);

MCGE_API const struct tmap* scene_get_tmap(void);
MCGE_API const struct chunks* scene_get_chunks(void);

MCGE_API struct entity* scene_try_get_entity(uint128_t id);
MCGE_API void scene_capture_entity(struct entity *entity);

#endif //MYCOOLGAMEENGINE_SCENE_H
