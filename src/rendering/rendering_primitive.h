//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_H
#define MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_H

#include "../utils/rect.h"

struct SDL_Renderer;
struct rendering_primitive;

void rendering_primitive_draw(struct rendering_primitive* self, struct rect rect, struct rect viewport, struct SDL_Renderer* renderer);

#endif //MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_H
