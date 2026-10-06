//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_H
#define MYCOOLGAMEENGINE_TEXTURE_H

enum texture_loading_mode {
    texture_loading_mode_eager,
    texture_loading_mode_lazy
};

struct texture;
struct SDL_Renderer;
struct asset_type;

extern const struct asset_type texture_asset_type;

struct texture* texture_asset_get(const char *name);

struct texture* texture_create(char* id, char* path, int tile_w, int tile_h, int tiles_count, enum texture_loading_mode loading_mode, struct SDL_Renderer* native_renderer);
void texture_destroy(struct texture* this);

const char* texture_get_id(const struct texture* this);

int texture_get_tiles_count(const struct texture* this);
void texture_get_tile_rect(struct texture* this, unsigned long long frame, float* target_x, float* target_y, float *target_w, float *target_h);

struct SDL_Texture* texture_get_native_texture(struct texture* this);

#endif //MYCOOLGAMEENGINE_TEXTURE_H
