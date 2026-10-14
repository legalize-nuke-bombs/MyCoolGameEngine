//
// Created by nikita on 14.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_TEXT_H
#define MYCOOLGAMEENGINE_RENDERER_TEXT_H

#include "mcge/api.h"
#include "mcge/utils/color.h"

struct font;
struct renderer_text;

MCGE_API struct renderer_primitive* renderer_text_create_from_string(const char *string, struct color color, struct font *font);

MCGE_API void renderer_text_set_string(const struct renderer_text *this, const char *string);

#endif //MYCOOLGAMEENGINE_RENDERER_TEXT_H
