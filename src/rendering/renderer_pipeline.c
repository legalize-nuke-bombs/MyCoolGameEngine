//
// Created by nikita on 23.09.2026.
//

#include "renderer_pipeline.h"

#include <stdbool.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "../logging/logger.h"
#include "../utils/action.h"
#include "custom/light_map.h"


#define DRAW_CALLS_BUFFER_SIZE (1 << 14)
#define LAYER_PRIORITIES_COUNT (UINT8_MAX + 1)


struct renderer_pipeline {
    SDL_Renderer *native_renderer;

    struct rect viewport;
    bool viewport_enabled;

    struct renderer_pipeline_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    struct renderer_pipeline_draw_call sorted_draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;

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
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE) {
        logger_warn("Rendering pipeline draw calls buffer is full. The frame will be incomplete.");
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

static void renderer_pipeline_sort_draw_calls(struct renderer_pipeline *this) {
    int offsets[LAYER_PRIORITIES_COUNT] = {0};
    for (int i = 0; i < this->draw_calls_count; i++) {
        offsets[renderer_layer_get_priority(this->draw_calls[i].layer)]++;
    }
    int offset = 0;
    for (int priority = 0; priority < LAYER_PRIORITIES_COUNT; priority++) {
        const int count = offsets[priority];
        offsets[priority] = offset;
        offset += count;
    }
    for (int i = 0; i < this->draw_calls_count; i++) {
        this->sorted_draw_calls[offsets[renderer_layer_get_priority(this->draw_calls[i].layer)]++] = this->draw_calls[i];
    }
}

static void renderer_pipeline_update_viewport_resolution(struct renderer_pipeline *this) {
    int native_renderer_w, native_renderer_h;
    SDL_GetRenderOutputSize(this->native_renderer, &native_renderer_w, &native_renderer_h);
    this->viewport.size.x = native_renderer_w;
    this->viewport.size.y = native_renderer_h;
}

void renderer_pipeline_flush(struct renderer_pipeline *this) {
    renderer_pipeline_draw_primitive(this, (struct renderer_pipeline_draw_call) {
        .primitive = this->light_map,
        .rect = rect_0,
        .layer = NULL
    });

    if (!this->viewport_enabled) {
        this->draw_calls_count = 0;
        return;
    }

    renderer_pipeline_update_viewport_resolution(this);

    renderer_pipeline_sort_draw_calls(this);
    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct renderer_pipeline_draw_call draw_call = this->sorted_draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, this->viewport, this->native_renderer);
    }

    this->draw_calls_count = 0;
}

struct light_map* renderer_pipeline_get_light_map(const struct renderer_pipeline *this) {
    return (struct light_map*)this->light_map;
}