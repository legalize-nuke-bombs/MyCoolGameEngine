//
// Created by nikita on 14.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FONT_H
#define MYCOOLGAMEENGINE_FONT_H

#include "../../../api.h"

enum font_loading_mode {
    font_loading_mode_lazy,
    font_loading_mode_eager
};

MCGE_API struct font* font_create(char *id, char *path, enum font_loading_mode loading_mode);
MCGE_API void font_destroy(struct font* this);

MCGE_API struct TTF_Font* font_get_native_font(struct font* this);
MCGE_API struct TTF_Font* font_get_native_font_for_height(struct font* this, int height);

#endif //MYCOOLGAMEENGINE_FONT_H
