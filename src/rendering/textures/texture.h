//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_H
#define MYCOOLGAMEENGINE_TEXTURE_H

#include "texture_loading_mode.h"

struct texture;
struct SDL_Renderer;

struct texture* texture_create(char* id, char* path, int tile_w, int tile_h, enum texture_loading_mode loading_mode, double unload_interval, struct SDL_Renderer* native_renderer);
void texture_destroy(struct texture* this);

const char* texture_get_id(const struct texture* this);
void texture_get_tile_rect(struct texture* this, unsigned long long frame, float* target_x, float* target_y, float *target_w, float *target_h);
struct SDL_Texture* texture_get_native_texture(struct texture* this);

void texture_update(struct texture* this, double elapsed);

#endif //MYCOOLGAMEENGINE_TEXTURE_H
