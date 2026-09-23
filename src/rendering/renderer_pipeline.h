//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PIPELINE_H
#define MYCOOLGAMEENGINE_RENDERER_PIPELINE_H

#include "../utils/rect.h"
#include "rendering_primitive.h"
#include "rendering_layer.h"

struct renderer_pipeline;
struct SDL_Renderer;

struct renderer_pipeline* renderer_pipeline_create(struct SDL_Renderer *native_renderer);
void renderer_pipeline_destroy(struct renderer_pipeline *this);

void renderer_pipeline_set_viewport(struct renderer_pipeline *this, struct rect rect);
void renderer_pipeline_remove_viewport(struct renderer_pipeline *this);

struct renderer_pipeline_draw_call {
    struct rect rect;
    struct rendering_primitive primitive;
    struct rendering_layer layer;
};

void renderer_pipeline_draw_primitive(struct renderer_pipeline *this, struct renderer_pipeline_draw_call draw_call);

void renderer_pipeline_flush(struct renderer_pipeline *this);

#endif //MYCOOLGAMEENGINE_RENDERER_PIPELINE_H
