//
// Created by nikita on 23.09.2026.
//
#include "renderer_square.h"
#include "../renderer_primitive_internal.h"
#include "../../utils/rect_math.h"
#include "../../utils/vector2_math.h"
#include <stdlib.h>
#include <SDL3/SDL.h>

struct renderer_square {
    struct renderer_primitive base;
    struct color color;
};

static void renderer_square_draw(const void* self, const struct rect rect, const struct rect viewport, SDL_Renderer* renderer) {
    const struct renderer_square* this = (struct renderer_square*)self;

    const struct rect camera_position_engine = {
        .position = vector_sub(&rect.position, &viewport.position),
        .size = rect.size
    };
    const struct rect camera_position_sdl = rect_sdl(&camera_position_engine);

    const SDL_FRect sdl_rect = {
        .x = camera_position_sdl.position.x,
        .y = camera_position_sdl.position.y,
        .w = camera_position_sdl.size.x,
        .h = camera_position_sdl.size.y
    };

    SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
    SDL_RenderFillRect(renderer, &sdl_rect);
}

static const struct rendering_primitive_vtable renderer_square_vtable = {
    .draw = renderer_square_draw
};

struct renderer_square* renderer_square_create(const struct color color) {
    struct renderer_square* this = malloc(sizeof(struct renderer_square));
    this->base.vtable = &renderer_square_vtable;
    this->color = color;
    return this;
}

void renderer_square_destroy(struct renderer_square* this) {
    free(this);
}

struct renderer_primitive* renderer_square_as_renderer_primitive(struct renderer_square* this) {
    return (struct renderer_primitive*) this;
}
