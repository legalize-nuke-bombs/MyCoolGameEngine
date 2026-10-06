//
// Created by nikita on 23.09.2026.
//
#include <stdlib.h>

#include "renderer_primitive_internal.h"


void renderer_primitive_draw(struct renderer_primitive* this, const struct rect rect, const struct rect viewport, const struct vector2 output_size, struct SDL_Renderer* renderer) {
    this->vtable->draw(this, rect, viewport, output_size, renderer);
}

bool renderer_primitive_is_visible(struct renderer_primitive* this, const struct rect rect, const struct rect viewport) {
    return this->vtable->is_visible(this, rect, viewport);
}

void renderer_primitive_destroy(struct renderer_primitive* this) {
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}