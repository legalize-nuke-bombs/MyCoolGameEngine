//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_INTERNAL_H
#define MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_INTERNAL_H

#include "rendering_primitive.h"

struct rendering_primitive_vtable {
    void (*draw)(void* self, struct rect rect, struct rect viewport, struct SDL_Renderer* renderer);
};

struct rendering_primitive {
    const struct rendering_primitive_vtable* vtable;
};

#endif //MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_INTERNAL_H
