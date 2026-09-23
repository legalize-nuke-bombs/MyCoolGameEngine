//
// Created by nikita on 23.09.2026.
//

#include "renderer_pipeline.h"

#include <stdbool.h>
#include <stdlib.h>
#include "../logging/logger.h"


#define DRAW_CALLS_BUFFER_SIZE 1024


struct renderer_pipeline {
    struct SDL_Renderer *native_renderer;

    struct rect viewport;
    bool viewport_enabled;

    struct renderer_pipeline_draw_call draw_calls[DRAW_CALLS_BUFFER_SIZE];
    int draw_calls_count;
};


struct renderer_pipeline* renderer_pipeline_create(struct SDL_Renderer *native_renderer) {
    struct renderer_pipeline *this = malloc(sizeof(struct renderer_pipeline));
    this->native_renderer = native_renderer;
    this->viewport_enabled = false;
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
    if (!this->viewport_enabled) {
        logger_warn("Rendering pipeline viewport is not set.");
        return;
    }
    if (this->draw_calls_count >= DRAW_CALLS_BUFFER_SIZE) {
        logger_warn("Rendering pipeline draw calls buffer is full. The frame will be incomplete.");
        return;
    }
    this->draw_calls[this->draw_calls_count++] = draw_call;
}

static int renderer_pipeline_draw_call_compair(const void *draw_call1, const void *draw_call2) {
    const struct renderer_pipeline_draw_call* dc1 = (struct renderer_pipeline_draw_call*)draw_call1;
    const struct renderer_pipeline_draw_call* dc2 = (struct renderer_pipeline_draw_call*)draw_call2;
    int p1 = dc1->layer.priority;
    int p2 = dc2->layer.priority;
    return (p1 > p2) - (p1 < p2);
}

void renderer_pipeline_flush(struct renderer_pipeline *this) {
    qsort(this->draw_calls, this->draw_calls_count, sizeof(struct renderer_pipeline_draw_call), renderer_pipeline_draw_call_compair);
    for (int i = 0; i < this->draw_calls_count; i++) {
        struct renderer_pipeline_draw_call draw_call = this->draw_calls[i];
        draw_call.primitive.draw(draw_call.rect, this->viewport, this->native_renderer);
    }
    this->draw_calls_count = 0;
}