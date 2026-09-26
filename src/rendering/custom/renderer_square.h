//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_SQUARE_H
#define MYCOOLGAMEENGINE_RENDERER_SQUARE_H

#include "../../utils/color.h"

struct renderer_square;
struct SDL_Texture;

struct renderer_square* renderer_square_create_from_color(struct color color);
struct renderer_square* renderer_square_create_from_texture(struct SDL_Texture* texture);
void renderer_square_destroy(struct renderer_square* this);

struct renderer_primitive* renderer_square_as_renderer_primitive(struct renderer_square* this);

#endif //MYCOOLGAMEENGINE_RENDERER_SQUARE_H
