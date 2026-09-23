//
// Created by nikita on 23.09.2026.
//
#include "rendering_primitive_internal.h"


void rendering_primitive_draw(struct rendering_primitive* self, const struct rect rect, const struct rect viewport, struct SDL_Renderer* renderer) {
    self->vtable->draw(self, rect, viewport, renderer);
}