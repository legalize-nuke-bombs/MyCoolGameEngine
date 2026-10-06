//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_RENDERER_H
#define MYCOOLGAMEENGINE_BOX_RENDERER_H

#include "../../../api.h"


struct box_renderer;
struct entity;
struct component_vtable;

MCGE_API const char* box_renderer_component_key(void);

MCGE_API extern const struct component_vtable box_renderer_vtable;

MCGE_API void box_renderer_bump_texture_frame(const struct box_renderer *this);
MCGE_API int box_renderer_get_texture_frames(const struct box_renderer *this);

#endif //MYCOOLGAMEENGINE_BOX_RENDERER_H
