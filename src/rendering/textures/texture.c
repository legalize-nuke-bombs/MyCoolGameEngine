//
// Created by Nikita on 27.09.2026.
//

#include "texture.h"

#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../../logging/logger.h"

struct texture {
    char* id;
    char* path;

    bool loading_failed;

    double unload_timer;
    double unload_interval;

    SDL_Texture* native_texture;

    SDL_Renderer* native_renderer;
};

static void texture_load(struct texture* this);

struct texture* texture_create(char* id, char* path, const enum texture_loading_mode loading_mode, const double unload_interval, SDL_Renderer* native_renderer) {
    logger_debug("Texture %s is creating...", id);
    struct texture* this = calloc(1, sizeof(struct texture));
    this->id = id;
    this->path = path;
    this->unload_interval = unload_interval;
    this->native_renderer = native_renderer;
    if (loading_mode == texture_loading_mode_eager) {
        texture_load(this);
    }
    return this;
}
void texture_destroy(struct texture* this) {
    logger_debug("Texture %s is destroying...", this->id);
    free(this->id);
    free(this->path);
    if (this->native_texture != NULL) {
        SDL_DestroyTexture(this->native_texture);
    }
    free(this);
}

const char* texture_get_id(const struct texture* this) {
    return this->id;
}
SDL_Texture* texture_get_native_texture(struct texture* this) {
    this->unload_timer = 0;
    if (this->native_texture != NULL) {
        return this->native_texture;
    }
    texture_load(this);
    return this->native_texture;
}

static void texture_load(struct texture* this) {
    if (this->native_texture != NULL) {
        return;
    }
    if (this->loading_failed) {
        return;
    }
    logger_debug("Texture %s is loading from %s...", this->id, this->path);

    this->native_texture = IMG_LoadTexture(this->native_renderer, this->path);
    if (this->native_texture == NULL) {
        logger_warn("Texture %s failed to load from %s", this->id, this->path);
        this->loading_failed = true;
    }
}

void texture_update(struct texture *this, const double dt) {
    if (this->native_texture == NULL) {
        return;
    }
    if (this->unload_interval < 0) {
        return;
    }
    this->unload_timer += dt;
    if (this->unload_timer >= this->unload_interval) {
        logger_debug("Texture %s will be unloaded because it was not used in %f seconds", this->id, this->unload_interval);
        this->unload_timer = 0;
        SDL_DestroyTexture(this->native_texture);
        this->native_texture = NULL;
    }
}