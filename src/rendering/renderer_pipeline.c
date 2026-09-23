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

void renderer_pipeline_set_viewport(struct renderer_pipeline *this, const struct rect rect) {
    this->viewport = rect;
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
        offsets[this->draw_calls[i].layer.priority]++;
    }
    int offset = 0;
    for (int priority = 0; priority < LAYER_PRIORITIES_COUNT; priority++) {
        const int count = offsets[priority];
        offsets[priority] = offset;
        offset += count;
    }
    for (int i = 0; i < this->draw_calls_count; i++) {
        this->sorted_draw_calls[offsets[this->draw_calls[i].layer.priority]++] = this->draw_calls[i];
    }
}

static void renderer_pipeline_present_viewport(struct renderer_pipeline *this) {
    const int width = (int) this->viewport.size.x;
    const int height = (int) this->viewport.size.y;
    if (width == this->presentation_width && height == this->presentation_height) {
        return;
    }
    SDL_SetRenderLogicalPresentation(this->native_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    this->presentation_width = width;
    this->presentation_height = height;
}

void renderer_pipeline_flush(struct renderer_pipeline *this) {
    if (!this->viewport_enabled) {
        logger_warn("Rendering pipeline viewport is not set.");
        this->draw_calls_count = 0;
        return;
    }
    renderer_pipeline_present_viewport(this);
    renderer_pipeline_sort_draw_calls(this);
    for (int i = 0; i < this->draw_calls_count; i++) {
        const struct renderer_pipeline_draw_call draw_call = this->sorted_draw_calls[i];
        renderer_primitive_draw(draw_call.primitive, draw_call.rect, this->viewport, this->native_renderer);
    }
    this->draw_calls_count = 0;
}
