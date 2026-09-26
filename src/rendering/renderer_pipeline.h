//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PIPELINE_H
#define MYCOOLGAMEENGINE_RENDERER_PIPELINE_H

#include "../utils/rect.h"
#include "renderer_primitive.h"
#include "renderer_layer.h"

struct renderer_pipeline;
struct SDL_Renderer;
struct renderer_layer;

struct renderer_pipeline* renderer_pipeline_create(struct SDL_Renderer *native_renderer);
void renderer_pipeline_destroy(struct renderer_pipeline *this);

void renderer_pipeline_set_viewpoint(struct renderer_pipeline *this, struct vector2 viewpoint);
void renderer_pipeline_remove_viewport(struct renderer_pipeline *this);

struct renderer_pipeline_draw_call {
    struct rect rect;
    struct renderer_primitive* primitive;
    struct renderer_layer* layer;
};

void renderer_pipeline_draw_primitive(struct renderer_pipeline *this, struct renderer_pipeline_draw_call draw_call);

void renderer_pipeline_flush(struct renderer_pipeline *this);

void renderer_pipeline_reset(struct renderer_pipeline *this);

struct SDL_Renderer* renderer_pipeline_get_native_renderer(const struct renderer_pipeline *this);

struct action* renderer_pipeline_on_viewpoint_resize(const struct renderer_pipeline *this);
struct action* renderer_pipeline_on_post_process(const struct renderer_pipeline *this);

struct light_map* renderer_pipeline_get_light_map(const struct renderer_pipeline *this);

#endif //MYCOOLGAMEENGINE_RENDERER_PIPELINE_H
