//
// Created by nikita on 23.09.2026.
//

#include "renderer_pipeline.h"

#include <stdbool.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "../logging/logger.h"
#include "primitives/custom/light_map.h"


#define DRAW_CALLS_BUFFER_SIZE (1 << 14)
#define LAYER_PRIORITIES_COUNT (UINT8_MAX + 1)


struct renderer_pipeline {
    SDL_Renderer *native_renderer;

    struct rect viewport;
    bool viewport_enabled;

    struct renderer_pipeline_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;
    int dropped_draw_calls_count;

    struct renderer_primitive *light_map;
    struct renderer_layer *light_map_layer;
};


struct renderer_pipeline* renderer_pipeline_create(SDL_Renderer *native_renderer) {
    logger_info("Renderer pipeline is creating...");
    struct renderer_pipeline *this = calloc(1, sizeof(struct renderer_pipeline));
    this->native_renderer = native_renderer;
    this->light_map = light_map_create();
    this->light_map_layer = renderer_layer_create(strdup("Light Map Layer"), 100);
    return this;
}
void renderer_pipeline_destroy(struct renderer_pipeline *this) {
    logger_info("Renderer pipeline is destroying...");
    renderer_primitive_destroy(this->light_map);
    renderer_layer_destroy(this->light_map_layer);
    free(this);
}

void renderer_pipeline_set_viewpoint(struct renderer_pipeline *this, const struct vector2 viewpoint) {
    this->viewport.position = viewpoint;
    this->viewport_enabled = true;
}
void renderer_pipeline_remove_viewport(struct renderer_pipeline *this) {
    this->viewport_enabled = false;
}


void renderer_pipeline_draw_primitive(struct renderer_pipeline *this, struct renderer_pipeline_draw_call draw_call) {
    if (!renderer_primitive_is_visible(draw_call.primitive, draw_call.rect, this->viewport)) {
        return;
    }
    // The last slot is reserved for the light map
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE - 1) {
        this->dropped_draw_calls_count++;
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

static void renderer_pipeline_update_viewport_resolution(struct renderer_pipeline *this) {
    int native_renderer_w, native_renderer_h;
    SDL_GetRenderOutputSize(this->native_renderer, &native_renderer_w, &native_renderer_h);
    this->viewport.size.x = native_renderer_w;
    this->viewport.size.y = native_renderer_h;
}
struct rect renderer_pipeline_get_viewport(struct renderer_pipeline *this) {
    renderer_pipeline_update_viewport_resolution(this);
    return this->viewport;
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

    if (!this->viewport_enabled) {
        light_map_clear_draw_calls((struct light_map*)this->light_map);
        this->draw_calls_count = 0;
        return;
    }

    this->draw_calls[this->draw_calls_count++] = (struct renderer_pipeline_draw_call) {
        .primitive = this->light_map,
        .rect = rect_0,
        .layer = this->light_map_layer
    };

    renderer_pipeline_update_viewport_resolution(this);

    qsort(
           this->draw_calls,
           this->draw_calls_count,
           sizeof(struct renderer_pipeline_draw_call),
           draw_calls_compare
       );

    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct renderer_pipeline_draw_call draw_call = this->draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, this->viewport, this->native_renderer);
    }

    this->draw_calls_count = 0;
}

struct light_map* renderer_pipeline_get_light_map(const struct renderer_pipeline *this) {
    return (struct light_map*)this->light_map;
}