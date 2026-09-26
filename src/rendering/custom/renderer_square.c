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
    SDL_Texture* texture;
};

static void renderer_square_draw(const void* self, const struct rect rect, const struct rect viewport, SDL_Renderer* renderer) {
    const struct renderer_square* this = (struct renderer_square*)self;

    const struct rect target_rect = rect_sdl(&rect, &viewport);
    const SDL_FRect sdl_target_rect = {
        .x = target_rect.position.x,
        .y = target_rect.position.y,
        .w = target_rect.size.x,
        .h = target_rect.size.y
    };

    if (this->texture) {
        SDL_RenderTexture(renderer, this->texture, NULL, &sdl_target_rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
        SDL_RenderFillRect(renderer, &sdl_target_rect);
    }
}

static const struct rendering_primitive_vtable renderer_square_vtable = {
    .draw = renderer_square_draw
};

struct renderer_square* renderer_square_create_from_color(const struct color color) {
    struct renderer_square* this = calloc(1, sizeof(struct renderer_square));
    this->base.vtable = &renderer_square_vtable;
    this->color = color;
    return this;
}
struct renderer_square* renderer_square_create_from_texture(SDL_Texture* texture) {
    struct renderer_square* this = calloc(1, sizeof(struct renderer_square));
    this->base.vtable = &renderer_square_vtable;
    this->texture = texture;
    return this;
}

void renderer_square_destroy(struct renderer_square* this) {
    free(this);
}

struct renderer_primitive* renderer_square_as_renderer_primitive(struct renderer_square* this) {
    return (struct renderer_primitive*) this;
}
