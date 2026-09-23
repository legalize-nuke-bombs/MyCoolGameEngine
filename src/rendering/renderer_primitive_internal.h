//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_INTERNAL_H
#define MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_INTERNAL_H

#include "renderer_primitive.h"

struct rendering_primitive_vtable {
    void (*draw)(const void* self, const struct rect rect, const struct rect viewport, struct SDL_Renderer* renderer);
};

struct renderer_primitive {
    const struct rendering_primitive_vtable* vtable;
};

#endif //MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_INTERNAL_H
