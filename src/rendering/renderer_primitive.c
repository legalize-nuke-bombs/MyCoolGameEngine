//
// Created by nikita on 23.09.2026.
//
#include "renderer_primitive_internal.h"


void renderer_primitive_draw(struct renderer_primitive* self, const struct rect rect, const struct rect viewport, struct SDL_Renderer* renderer) {
    self->vtable->draw(self, rect, viewport, renderer);
}