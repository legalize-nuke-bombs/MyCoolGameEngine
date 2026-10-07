//
// Created by nikita on 23.09.2026.
//
#include "renderer_square.h"
#include "../renderer_primitive_internal.h"
#include "../../../utils/rect_math.h"
#include "../../../utils/vector2_math.h"
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "../../texture.h"

struct renderer_square {
    struct renderer_primitive base;

    struct color color;

    struct texture* texture;
    unsigned long long texture_frame;

    bool flip_x, flip_y;
};

static void renderer_square_draw(struct renderer_primitive* base, const struct rect rect, const struct rect viewport, const struct vector2 output_size, SDL_Renderer* renderer) {
    const struct renderer_square* this = (struct renderer_square*)base;

    const struct rect target_rect = rect_sdl(&rect, &viewport, output_size);
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

        SDL_RenderTextureRotated(renderer, texture_get_native_texture(this->texture), &src_rect, &sdl_target_rect,
            0, NULL,
            this->flip_x && this->flip_y ? SDL_FLIP_HORIZONTAL_AND_VERTICAL : (this->flip_x ? SDL_FLIP_HORIZONTAL : (this->flip_y ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
            );
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

MCGE_API void renderer_square_set_color(struct renderer_square* this, const struct color color) {
    this->color = color;
}

void renderer_square_bump_texture_frame(struct renderer_square* this) {
    this->texture_frame++;
}
int renderer_square_get_texture_frames(const struct renderer_square* this) {
    if (this->texture == NULL) {
        return 0;
    }
    return texture_get_tiles_count(this->texture);
}

bool renderer_square_get_flip_x(const struct renderer_square* this) {
    return this->flip_x;
}
bool renderer_square_get_flip_y(const struct renderer_square* this) {
    return this->flip_y;
}
void renderer_square_set_flip_x(struct renderer_square* this, const bool value) {
    this->flip_x = value;
}
void renderer_square_set_flip_y(struct renderer_square* this, const bool value) {
    this->flip_y = value;
}
