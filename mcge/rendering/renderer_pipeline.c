//
// Created by nikita on 23.09.2026.
//

#include "renderer_pipeline.h"

#include <stdbool.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "../logging/logger.h"
#include "primitives/custom/light_map.h"
#include "../utils/action.h"


#define DRAW_CALLS_BUFFER_SIZE (1 << 14)
#define LAYER_PRIORITIES_COUNT (UINT8_MAX + 1)


struct renderer_pipeline {
    SDL_Renderer *native_renderer;

    struct rect viewport;
    bool viewport_enabled;

    struct vector2 output_size;

    struct renderer_pipeline_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int prev_draw_calls_count;
    int draw_calls_count;
    int dropped_draw_calls_count;

    struct action on_viewport_changed;
    struct action on_output_size_changed;

    struct renderer_primitive *light_map;
    struct renderer_layer *light_map_layer;
};


struct renderer_pipeline* renderer_pipeline_create(SDL_Renderer *native_renderer) {
    logger_info("Renderer pipeline is creating...");
    struct renderer_pipeline *this = allocator_new0(1, sizeof(struct renderer_pipeline));
    this->native_renderer = native_renderer;
    this->on_viewport_changed = action_create();
    this->on_output_size_changed = action_create();
    this->light_map = light_map_create();
    this->light_map_layer = renderer_layer_create(allocator_strcpy("Light Map Layer"), 100);
    return this;
}
void renderer_pipeline_destroy(struct renderer_pipeline *this) {
    logger_info("Renderer pipeline is destroying...");
    renderer_primitive_destroy(this->light_map);
    renderer_layer_destroy(this->light_map_layer);
    action_destroy(&this->on_output_size_changed);
    action_destroy(&this->on_viewport_changed);
    allocator_delete(this);
}

void renderer_pipeline_enable(struct renderer_pipeline *this) {

}
void renderer_pipeline_disable(struct renderer_pipeline *this) {
    light_map_reset((struct light_map*)this->light_map);
    action_clear(&this->on_output_size_changed);
    action_clear(&this->on_viewport_changed);
}

void renderer_pipeline_set_viewport(struct renderer_pipeline *this, const struct rect viewport) {
    if (this->viewport_enabled && rects_equal(this->viewport, viewport)) {
        return;
    }
    this->viewport = viewport;
    this->viewport_enabled = true;
    action_invoke(&this->on_viewport_changed, &this->viewport);
}
void renderer_pipeline_remove_viewport(struct renderer_pipeline *this) {
    this->viewport_enabled = false;
}
struct rect renderer_pipeline_get_viewport(const struct renderer_pipeline *this) {
    return this->viewport_enabled ? this->viewport : rect_0;
}

struct vector2 renderer_pipeline_get_output_size(const struct renderer_pipeline *this) {
    int native_renderer_w, native_renderer_h;
    SDL_GetRenderOutputSize(this->native_renderer, &native_renderer_w, &native_renderer_h);
    const struct vector2 output_size = {
        .x = native_renderer_w,
        .y = native_renderer_h
    };
    return output_size;
}


void renderer_pipeline_draw_primitive(struct renderer_pipeline *this, struct renderer_pipeline_draw_call draw_call) {
    if (!renderer_primitive_is_visible(draw_call.primitive, draw_call.rect, this->viewport)) {
        return;
    }
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE - 1) {
        this->dropped_draw_calls_count++;
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

static int draw_calls_compare(const void *a, const void *b) {
    const struct renderer_pipeline_draw_call *dc_a = a;
    const struct renderer_pipeline_draw_call *dc_b = b;

    const int dc_a_layer = renderer_layer_get_priority(dc_a->layer);
    const int dc_b_layer = renderer_layer_get_priority(dc_b->layer);

    if (dc_a_layer != dc_b_layer) {
        return (dc_a_layer > dc_b_layer) - (dc_a_layer < dc_b_layer);
    }

    const double dc_a_y = dc_a->rect.position.y;
    const double dc_b_y = dc_b->rect.position.y;

    if (dc_a_y != dc_b_y) {
        return (dc_a_y < dc_b_y) - (dc_a_y > dc_b_y);
    }

    const void *dc_a_ptr = dc_a->primitive;
    const void *dc_b_ptr = dc_b->primitive;

    if (dc_a_ptr != dc_b_ptr) {
        return (dc_a_ptr > dc_b_ptr) - (dc_a_ptr < dc_b_ptr);
    }

    return 0;
}

void renderer_pipeline_flush(struct renderer_pipeline *this) {
    if (this->dropped_draw_calls_count > 0) {
        logger_warn("Rendering pipeline draw calls buffer is full, %d draw calls dropped", this->dropped_draw_calls_count);
        this->dropped_draw_calls_count = 0;
    }

    struct vector2 output_size = renderer_pipeline_get_output_size(this);
    if (!vectors_equal(output_size, this->output_size)) { // TODO Наверное чтобы убрать задержку в 1 кадр в идеале не спрашивать постоянно размер аутпута а подписываться на нативные события SDL.
        action_invoke(&this->on_output_size_changed, &output_size);
        this->output_size = output_size;
    }

    if (!this->viewport_enabled) {
        light_map_clear_draw_calls((struct light_map*)this->light_map);
        this->draw_calls_count = 0;
        this->prev_draw_calls_count = 0;
        const char* no_active_camera_label = "NO ACTIVE CAMERA IN THE SCENE";
        const float debug_char_size = 8;
        SDL_SetRenderDrawColor(this->native_renderer, color_white.r, color_white.g, color_white.b, color_white.a);
        SDL_RenderDebugText(this->native_renderer, (float)(output_size.x * 0.5) - (float)strlen(no_active_camera_label) * debug_char_size / 2.f, (float)(output_size.y * 0.5) - debug_char_size / 2.f, no_active_camera_label);
        return;
    }

    this->draw_calls[this->draw_calls_count++] = (struct renderer_pipeline_draw_call) {
        .primitive = this->light_map,
        .rect = rect_0,
        .layer = this->light_map_layer
    };

    qsort(
           this->draw_calls,
           this->draw_calls_count,
           sizeof(struct renderer_pipeline_draw_call),
           draw_calls_compare
       );

    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct renderer_pipeline_draw_call draw_call = this->draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, this->viewport, output_size, this->native_renderer);
    }

    this->prev_draw_calls_count = this->draw_calls_count;
    this->draw_calls_count = 0;
}

struct light_map* renderer_pipeline_get_light_map(const struct renderer_pipeline *this) {
    return (struct light_map*)this->light_map;
}

struct action* renderer_pipeline_get_on_viewport_changed(struct renderer_pipeline *this) {
    return &this->on_viewport_changed;
}
struct action* renderer_pipeline_get_on_output_size_changed(struct renderer_pipeline *this) {
    return &this->on_output_size_changed;
}



int renderer_pipeline_get_prev_draw_call_count(const struct renderer_pipeline *this) {
    return this->prev_draw_calls_count;
}
