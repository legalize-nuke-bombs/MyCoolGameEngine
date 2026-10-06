//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_H
#define MYCOOLGAMEENGINE_TEXTURE_H

#include "../api.h"

enum texture_loading_mode {
    texture_loading_mode_eager,
    texture_loading_mode_lazy
};

struct texture;
struct SDL_Renderer;

MCGE_API struct texture* texture_create(char* id, char* path, int tile_w, int tile_h, int tiles_count, enum texture_loading_mode loading_mode, struct SDL_Renderer* native_renderer);
MCGE_API void texture_destroy(struct texture* this);

MCGE_API const char* texture_get_id(const struct texture* this);

MCGE_API int texture_get_tiles_count(const struct texture* this);
MCGE_API void texture_get_tile_rect(struct texture* this, unsigned long long frame, float* target_x, float* target_y, float *target_w, float *target_h);

MCGE_API struct SDL_Texture* texture_get_native_texture(struct texture* this);

#endif //MYCOOLGAMEENGINE_TEXTURE_H
