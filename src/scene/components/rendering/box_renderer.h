//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_BOX_RENDERER_H
#define MYCOOLGAMEENGINE_BOX_RENDERER_H


struct box_renderer;
struct entity;
struct component_vtable;

const char* box_renderer_component_key(void);

extern const struct component_vtable box_renderer_vtable;

void box_renderer_bump_texture_frame(const struct box_renderer *this);
int box_renderer_get_texture_frames(const struct box_renderer *this);

#endif //MYCOOLGAMEENGINE_BOX_RENDERER_H
