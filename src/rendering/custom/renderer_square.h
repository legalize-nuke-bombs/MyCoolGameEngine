//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_SQUARE_H
#define MYCOOLGAMEENGINE_RENDERER_SQUARE_H

#include "SDL3/SDL_pixels.h"

struct renderer_square;

struct renderer_square* renderer_square_create(SDL_Color color);
void renderer_square_destroy(struct renderer_square* this);

struct renderer_primitive* renderer_square_as_renderer_primitive(struct renderer_square* this);

#endif //MYCOOLGAMEENGINE_RENDERER_SQUARE_H
