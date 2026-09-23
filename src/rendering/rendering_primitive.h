//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_H
#define MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_H

struct SDL_Renderer;

struct rendering_primitive {
    void (*draw)(struct rect rect, struct rect viewport, struct SDL_Renderer* native_renderer);
};

#endif //MYCOOLGAMEENGINE_RENDERING_PRIMITIVE_H
