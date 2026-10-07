//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_SQUARE_H
#define MYCOOLGAMEENGINE_RENDERER_SQUARE_H

#include <stdbool.h>

#include "../../../utils/color.h"
#include "../../../api.h"

struct renderer_square;
struct SDL_Texture;
struct texture;

MCGE_API struct renderer_primitive* renderer_square_create_from_color(struct color color);
MCGE_API struct renderer_primitive* renderer_square_create_from_texture(struct texture* texture, int frame);

MCGE_API void renderer_square_bump_texture_frame(struct renderer_square* this);
MCGE_API int renderer_square_get_texture_frames(const struct renderer_square* this);

MCGE_API bool renderer_square_get_flip_x(const struct renderer_square* this);
MCGE_API bool renderer_square_get_flip_y(const struct renderer_square* this);
MCGE_API void renderer_square_set_flip_x(struct renderer_square* this, bool value);
MCGE_API void renderer_square_set_flip_y(struct renderer_square* this, bool value);

#endif //MYCOOLGAMEENGINE_RENDERER_SQUARE_H
