//
// Created by nikita on 23.09.2026.
//

#include "renderer_pipeline.h"

#include <stdbool.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "../logging/logger.h"


#define DRAW_CALLS_BUFFER_SIZE 1024
#define LAYER_PRIORITIES_COUNT (UINT8_MAX + 1)


struct renderer_pipeline {
    struct SDL_Renderer *native_renderer;

    struct rect viewport;
    bool viewport_enabled;
    int presentation_width;
    int presentation_height;

    struct renderer_pipeline_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    struct renderer_pipeline_draw_call sorted_draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;
};


struct renderer_pipeline* renderer_pipeline_create(struct SDL_Renderer *native_renderer) {
    struct renderer_pipeline *this = malloc(sizeof(struct renderer_pipeline));
    this->native_renderer = native_renderer;
    this->viewport_enabled = false;
    this->presentation_width = 0;
    this->presentation_height = 0;
    this->draw_calls_count = 0;
    return this;
}
void renderer_pipeline_destroy(struct renderer_pipeline *this) {
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
