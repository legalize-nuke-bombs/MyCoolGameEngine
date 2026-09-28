//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_H
#define MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_H

#include "../../utils/rect.h"

struct SDL_Renderer;
struct renderer_primitive;

void renderer_primitive_draw(struct renderer_primitive* this, struct rect rect, struct rect viewport, struct SDL_Renderer* renderer);

bool renderer_primitive_is_visible(struct renderer_primitive* this, struct rect rect, struct rect viewport);

void renderer_primitive_destroy(struct renderer_primitive* this);

#endif //MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_H
