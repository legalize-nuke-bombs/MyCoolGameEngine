//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_PIPELINE_H
#define MYCOOLGAMEENGINE_RENDERER_PIPELINE_H

#include "../utils/rect.h"
#include "primitives/renderer_primitive.h"
#include "renderer_layer.h"
#include "../api.h"

struct renderer_pipeline;
struct SDL_Renderer;
struct renderer_layer;

MCGE_API struct renderer_pipeline* renderer_pipeline_create(struct SDL_Renderer *native_renderer);
MCGE_API void renderer_pipeline_destroy(struct renderer_pipeline *this);

MCGE_API void renderer_pipeline_enable(struct renderer_pipeline *this);
MCGE_API void renderer_pipeline_disable(const struct renderer_pipeline *this);

MCGE_API void renderer_pipeline_set_viewport(struct renderer_pipeline *this, struct rect viewport);
MCGE_API void renderer_pipeline_remove_viewport(struct renderer_pipeline *this);
MCGE_API struct rect renderer_pipeline_get_viewport(const struct renderer_pipeline *this);

MCGE_API struct vector2 renderer_pipeline_get_output_size(const struct renderer_pipeline *this);

struct renderer_pipeline_draw_call {
    struct rect rect;
    struct renderer_primitive* primitive;
    struct renderer_layer* layer;
};

MCGE_API void renderer_pipeline_draw_primitive(struct renderer_pipeline *this, struct renderer_pipeline_draw_call draw_call);

MCGE_API void renderer_pipeline_flush(struct renderer_pipeline *this);

MCGE_API struct light_map* renderer_pipeline_get_light_map(const struct renderer_pipeline *this);

#endif //MYCOOLGAMEENGINE_RENDERER_PIPELINE_H
