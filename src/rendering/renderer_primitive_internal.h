//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_INTERNAL_H
#define MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_INTERNAL_H

#include "renderer_primitive.h"

struct rendering_primitive_vtable {
    void (*draw)(struct renderer_primitive* base, struct rect rect, struct rect viewport, struct SDL_Renderer* renderer);
    void (*on_destroy)(struct renderer_primitive* base);
};

struct renderer_primitive {
    const struct rendering_primitive_vtable* vtable;
};

#endif //MYCOOLGAMEENGINE_RENDERER_PRIMITIVE_INTERNAL_H
