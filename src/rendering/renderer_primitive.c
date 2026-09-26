//
// Created by nikita on 23.09.2026.
//
#include <stdlib.h>

#include "renderer_primitive_internal.h"


void renderer_primitive_draw(struct renderer_primitive* self, const struct rect rect, const struct rect viewport, struct SDL_Renderer* renderer) {
    self->vtable->draw(self, rect, viewport, renderer);
}

void renderer_primitive_destroy(struct renderer_primitive* this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}