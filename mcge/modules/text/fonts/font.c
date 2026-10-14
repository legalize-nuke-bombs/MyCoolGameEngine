//
// Created by nikita on 14.10.2026.
//

#include "font.h"
#include <stdlib.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "mcge/logging/logger.h"

struct font {
    char* id;
    char* path;
    float size;
    TTF_Font* native_font;
};


struct font* font_create(char *id, char *path, const float size, const enum font_loading_mode loading_mode) {
    struct font* this = calloc(1, sizeof(struct font));
    this->id = id;
    this->path = path;
    this->size = size;
    if (loading_mode == font_loading_mode_eager) {
        font_get_native_font(this);
    }
    return this;
}
void font_destroy(struct font* this) {
    free(this->id);
    free(this->path);
    if (this->native_font) TTF_CloseFont(this->native_font);
    free(this);
}

TTF_Font* font_get_native_font(struct font* this) {
    if (this->native_font == NULL) {
        logger_debug("Font %s is loading from %s (size %f)...", this->id, this->path, this->size);
        this->native_font = TTF_OpenFont(this->path, this->size);
        if (!this->native_font) {
            logger_warn("Failed to load font %s from %s (size %f)", this->id, this->path, this->size);
        }
    }
    return this->native_font;
}