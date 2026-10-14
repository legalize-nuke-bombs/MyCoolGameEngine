//
// Created by nikita on 14.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_TEXT_H
#define MYCOOLGAMEENGINE_RENDERER_TEXT_H

#include "mcge/api.h"

struct font;

MCGE_API struct renderer_primitive* renderer_text_create_from_string(const char *string, struct font *font);

#endif //MYCOOLGAMEENGINE_RENDERER_TEXT_H
