//
// Created by nikita on 14.10.2026.
//

#include "font.h"
#include <stdlib.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "mcge/logging/logger.h"

#define FONT_BASE_SIZE 100.0f

struct font_size {
    int height;
    TTF_Font* native_font;
};

#define LIST_NAME font_sizes_list
#define LIST_TYPE struct font_size
#include "mcge/utils/typed_list.h"

struct font {
    char* id;
    char* path;
    TTF_Font* native_font;
    struct font_sizes_list sizes;
};


struct font* font_create(char *id, char *path, const enum font_loading_mode loading_mode) {
    struct font* this = allocator_new0(1, sizeof(struct font));
    this->id = id;
    this->path = path;
    this->sizes = font_sizes_list_create(4);
    if (loading_mode == font_loading_mode_eager) {
        font_get_native_font(this);
    }
    return this;
}
void font_destroy(struct font* this) {
    for (int i = 0; i < font_sizes_list_count(&this->sizes); i++) {
        TTF_CloseFont(font_sizes_list_get(&this->sizes, i)->native_font);
    }
    font_sizes_list_destroy(&this->sizes);
    allocator_delete(this->id);
    allocator_delete(this->path);
    if (this->native_font) TTF_CloseFont(this->native_font);
    allocator_delete(this);
}

TTF_Font* font_get_native_font(struct font* this) {
    if (this->native_font == NULL) {
        logger_debug("Font %s is loading from %s...", this->id, this->path);
        this->native_font = TTF_OpenFont(this->path, FONT_BASE_SIZE);
        if (!this->native_font) {
            logger_warn("Failed to load font %s from %s", this->id, this->path);
        }
    }
    return this->native_font;
}

TTF_Font* font_get_native_font_for_height(struct font* this, const int height) {
    for (int i = 0; i < font_sizes_list_count(&this->sizes); i++) {
        const struct font_size* size = font_sizes_list_get(&this->sizes, i);
        if (size->height == height) {
            return size->native_font;
        }
    }

    TTF_Font* base = font_get_native_font(this);
    if (base == NULL) {
        return NULL;
    }
    TTF_Font* native_font = TTF_CopyFont(base);
    if (native_font == NULL) {
        logger_warn("Failed to copy font %s for height %d", this->id, height);
        return NULL;
    }
    TTF_SetFontSize(native_font, FONT_BASE_SIZE * (float)height / (float)TTF_GetFontHeight(base));

    const struct font_size size = {
        .height = height,
        .native_font = native_font
    };
    font_sizes_list_add(&this->sizes, size);
    logger_debug("Font %s got height %d (%d fonts)", this->id, height, font_sizes_list_count(&this->sizes));
    return native_font;
}
