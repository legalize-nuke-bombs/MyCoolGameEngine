//
// Created by nikita on 23.09.2026.
//
#include "renderer_square.h"
#include "../renderer_primitive_internal.h"
#include "../../../utils/rect_math.h"
#include "../../../utils/vector2_math.h"
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../../textures/texture.h"

struct renderer_square {
    struct renderer_primitive base;

    struct color color;

    struct texture* texture;
    unsigned long long texture_frame;
};

static void renderer_square_draw(struct renderer_primitive* base, const struct rect rect, const struct rect viewport, SDL_Renderer* renderer) {
    const struct renderer_square* this = (struct renderer_square*)base;

    const struct rect target_rect = rect_sdl(&rect, &viewport);
    const SDL_FRect sdl_target_rect = {
        .x = target_rect.position.x,
        .y = target_rect.position.y,
        .w = target_rect.size.x,
        .h = target_rect.size.y
    };

    if (this->texture) {
        SDL_FRect src_rect;
        texture_get_tile_rect(this->texture, this->texture_frame, &src_rect.x, &src_rect.y, &src_rect.w, &src_rect.h);

        SDL_BlendMode current_blend_mode;
        SDL_GetRenderDrawBlendMode(renderer, &current_blend_mode);
        SDL_SetTextureBlendMode(texture_get_native_texture(this->texture), current_blend_mode);

        SDL_RenderTexture(renderer, texture_get_native_texture(this->texture), &src_rect, &sdl_target_rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
        SDL_RenderFillRect(renderer, &sdl_target_rect);
    }
}

static bool renderer_square_is_visible(struct renderer_primitive* base, const struct rect rect, const struct rect viewport) {
    const struct renderer_square* this = (struct renderer_square*)base;

    return rects_intersection(rect, viewport);
}

static const struct rendering_primitive_vtable renderer_square_vtable = {
    .draw = renderer_square_draw,
    .is_visible = renderer_square_is_visible,
    .on_destroy = NULL
};

struct renderer_primitive* renderer_square_create_from_color(const struct color color) {
    struct renderer_square* this = calloc(1, sizeof(struct renderer_square));
    this->base.vtable = &renderer_square_vtable;
    this->color = color;
    return (struct renderer_primitive*)this;
}
struct renderer_primitive* renderer_square_create_from_texture(struct texture* texture, int frame) {
    struct renderer_square* this = calloc(1, sizeof(struct renderer_square));
    this->base.vtable = &renderer_square_vtable;
    this->texture = texture;
    this->texture_frame = frame;
    return (struct renderer_primitive*)this;
}

struct renderer_primitive* renderer_square_clone(const struct renderer_square* square) {
    struct renderer_square* this = calloc(1, sizeof(struct renderer_square));
    this->base.vtable = &renderer_square_vtable;
    this->color = square->color;
    this->texture = square->texture;
    this->texture_frame = square->texture_frame;
    return (struct renderer_primitive*)this;
}

void renderer_square_bump_texture_frame(struct renderer_square* this) {
    this->texture_frame++;
}
