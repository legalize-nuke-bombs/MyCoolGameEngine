//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_CONTEXT_H
#define MYCOOLGAMEENGINE_ENGINE_CONTEXT_H

struct engine_context;
struct renderer_pipeline;

struct engine_context* engine_context_create(struct renderer_pipeline *renderer_pipeline);
void engine_context_destroy(struct engine_context* this);

struct renderer_pipeline* engine_context_get_renderer_pipeline(const struct engine_context *this);

#endif //MYCOOLGAMEENGINE_ENGINE_CONTEXT_H
