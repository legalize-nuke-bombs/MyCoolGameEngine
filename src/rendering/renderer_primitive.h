//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_H
#define MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_H

#include "../utils/rect.h"

struct SDL_Renderer;
struct renderer_primitive;

void renderer_primitive_draw(struct renderer_primitive* self, struct rect rect, struct rect viewport, struct SDL_Renderer* renderer);

#endif //MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_H
