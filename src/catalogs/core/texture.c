//
// Created by Nikita on 27.09.2026.
//

#include "texture.h"

#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "../../rendering/renderer.h"
#include "../catalog.h"
#include "../../logging/logger.h"
#include "../../subsystems/subsystem_collection.h"
#include "../../utils/parser.h"

struct texture {
    char* id;
    char* path;

    float texture_w;
    float texture_h;
    int tiles_count;

    int tile_w;
    int tile_h;

    bool loading_failed;

    SDL_Texture* native_texture;

    SDL_Renderer* native_renderer;
};

static void texture_load(struct texture* this);

struct texture* texture_create(char* id, char* path, const int tile_w, const int tile_h, const int tiles_count, const enum texture_loading_mode loading_mode, SDL_Renderer* native_renderer) {
    logger_debug("Texture %s is creating...", id);
    struct texture* this = calloc(1, sizeof(struct texture));

    this->id = id;
    this->path = path;

    this->texture_w = -1;
    this->texture_h = -1;
    this->tiles_count = tiles_count;

    this->tile_w = tile_w;
    this->tile_h = tile_h;

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

static const char* texture_catalog_key(void) {
    return "texture";
}

static void* texture_on_create_item(const char *name, struct parser *parser, const struct subsystem_collection *subsystems) {
    int tile_w, tile_h, tiles_count;
    parser_next_int(parser, &tile_w);
    parser_next_int(parser, &tile_h);
    parser_next_int(parser, &tiles_count);
    char* path = parser_next_dup(parser);
    const char* loading_mode_name = parser_next(parser);

    enum texture_loading_mode loading_mode = texture_loading_mode_lazy;
    if (loading_mode_name != NULL && strcmp(loading_mode_name, "eager") == 0) {
        loading_mode = texture_loading_mode_eager;
    }
    else if (loading_mode_name == NULL || strcmp(loading_mode_name, "lazy") != 0) {
        logger_warn("Unexpected texture loading mode `%s`, `lazy` will be used instead", loading_mode_name ? loading_mode_name : "<null>");
    }

    const struct renderer* renderer = (struct renderer*)subsystem_collection_get(subsystems, "renderer");
    return texture_create(strdup(name), path, tile_w, tile_h, tiles_count, loading_mode, renderer_get_native_renderer(renderer));
}
static void texture_on_destroy_item(void *item) {
    texture_destroy(item);
}

const struct catalog_vtable texture_catalog_vtable = {
    .key = texture_catalog_key,
    .on_create_item = texture_on_create_item,
    .on_destroy_item = texture_on_destroy_item
};

const char* texture_get_id(const struct texture* this) {
    return this->id;
}
SDL_Texture* texture_get_native_texture(struct texture* this) {
    if (this->native_texture != NULL) {
        return this->native_texture;
    }
    texture_load(this);
    return this->native_texture;
}

int texture_get_tiles_count(const struct texture* this) {
    return this->tiles_count;
}
void texture_get_tile_rect(struct texture* this, const unsigned long long frame, float* target_x, float* target_y, float *target_w, float *target_h) {
    if (this->texture_w < 0 || this->texture_h < 0) {
        SDL_Texture* texture = texture_get_native_texture(this);
        SDL_GetTextureSize(texture, &this->texture_w, &this->texture_h);
    }

    const int tiles_count = texture_get_tiles_count(this);

    if (tiles_count <= 0 || this->tile_w <= 0 || this->tile_h <= 0 || this->texture_w <= 0) {
        *target_x = 0;
        *target_y = 0;
        *target_w = 0;
        *target_h = 0;
        return;
    }

    const unsigned long long safe_frame = frame % tiles_count;
    int columns = (int)this->texture_w / this->tile_w;
    if (columns <= 0) columns = 1;

    const int col = (int)(safe_frame % columns);
    const int row = (int)(safe_frame / columns);

    *target_x = (float)col * (float)this->tile_w;
    *target_y = (float)row * (float)this->tile_h;
    *target_w = (float)this->tile_w;
    *target_h = (float)this->tile_h;
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