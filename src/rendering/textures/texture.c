//
// Created by Nikita on 27.09.2026.
//

#include "texture.h"

#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../../logging/logger.h"
#include "../../utils/path.h"

struct texture {
    char* id;
    char* rpath;

    float texture_w;
    float texture_h;
    int tiles_count;

    int tile_w;
    int tile_h;

    enum texture_loading_mode mode;
    bool loading_failed;
    bool used;

    double unload_timer;
    double unload_interval;

    SDL_Texture* native_texture;

    const char* data_root;
    SDL_Renderer* native_renderer;
};

static void texture_load(struct texture* this);

struct texture* texture_create(char* id, char* rpath, const int tile_w, const int tile_h, const enum texture_loading_mode loading_mode, const double unload_interval, SDL_Renderer* native_renderer) {
    logger_debug("Texture %s is creating...", id);
    struct texture* this = calloc(1, sizeof(struct texture));

    this->id = id;
    this->rpath = rpath;

    this->texture_w = -1;
    this->texture_h = -1;
    this->tiles_count = -1;

    this->tile_w = tile_w;
    this->tile_h = tile_h;

    this->mode = loading_mode;

    this->unload_interval = unload_interval;

    this->native_renderer = native_renderer;

    return this;
}
void texture_destroy(struct texture* this) {
    logger_debug("Texture %s is destroying...", this->id);
    free(this->id);
    free(this->rpath);
    if (this->native_texture != NULL) {
        SDL_DestroyTexture(this->native_texture);
    }
    free(this);
}

void texture_set_data_root(struct texture* this, const char* data_root) {
    this->data_root = data_root;
    if (this->mode == texture_loading_mode_eager) {
        texture_load(this);
    }
}

const char* texture_get_id(const struct texture* this) {
    return this->id;
}
SDL_Texture* texture_get_native_texture(struct texture* this) {
    this->used = true;
    if (this->native_texture != NULL) {
        return this->native_texture;
    }
    texture_load(this);
    return this->native_texture;
}

static int texture_get_tiles_count(struct texture* this) {
    if (this->tiles_count < 0) {
        if (this->texture_w < 0 || this->texture_h < 0) {
            SDL_Texture* texture = texture_get_native_texture(this);
            if (texture == NULL) {
                this->texture_w = this->texture_h = 0;
            }
            else {
                SDL_GetTextureSize(texture, &this->texture_w, &this->texture_h);
            }
        }
        this->tiles_count = (int)this->texture_w / this->tile_w * (int)this->texture_h / this->tile_h;
    }
    return this->tiles_count;
}
void texture_get_tile_rect(struct texture* this, const unsigned long long frame, float* target_x, float* target_y, float *target_w, float *target_h) {
    int tiles_count = texture_get_tiles_count(this);
    if (tiles_count <= 0) {
        *target_x = 0;
        *target_y = 0;
        *target_w = 0;
        *target_h = 0;
    }
    else {
        *target_x = (float)(frame % texture_get_tiles_count(this)) * (float)this->tile_w;
        *target_y = 0;
        *target_w = (float)this->tile_w;
        *target_h = (float)this->tile_h;
    }
}

static void texture_load(struct texture* this) {
    if (this->native_texture != NULL) {
        return;
    }
    if (this->loading_failed) {
        return;
    }

    char* path = path_alloc_combined(this->data_root, this->rpath); // allocation here is free relative to IMG_LoadTexture
    logger_debug("Texture %s is loading from %s...", this->id, path);
    this->native_texture = IMG_LoadTexture(this->native_renderer, path);
    if (this->native_texture == NULL) {
        logger_warn("Texture %s failed to load from %s", this->id, path);
        this->loading_failed = true;
    }
    free(path);
}

void texture_update(struct texture *this, const double elapsed) {
    if (this->native_texture == NULL) {
        return;
    }
    if (this->unload_interval < 0) {
        return;
    }
    if (this->used) {
        this->used = false;
        this->unload_timer = 0;
        return;
    }
    this->unload_timer += elapsed;
    if (this->unload_timer >= this->unload_interval) {
        logger_debug("Texture %s will be unloaded because it was not used in %f seconds", this->id, this->unload_interval);
        this->unload_timer = 0;
        SDL_DestroyTexture(this->native_texture);
        this->native_texture = NULL;
    }
}