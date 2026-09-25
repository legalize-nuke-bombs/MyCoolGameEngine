//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_EXECUTION_CONTEXT_H
#define MYCOOLGAMEENGINE_ENGINE_EXECUTION_CONTEXT_H

#include "engine_execution_arguments.h"

struct engine;
struct engine_execution_context;

struct engine_execution_context* engine_execution_context_create(struct engine *engine, struct engine_execution_arguments arguments);
void engine_execution_context_destroy(struct engine_execution_context *this);

void engine_execution_context_awake(struct engine_execution_context *this);



bool engine_execution_context_is_running(const struct engine_execution_context *this);
void engine_execution_context_stop(struct engine_execution_context *this);

struct renderer_layer_manager* engine_execution_context_get_renderer_layer_manager(const struct engine_execution_context *this);
struct scene* engine_execution_context_get_scene(const struct engine_execution_context *this);



#endif //MYCOOLGAMEENGINE_ENGINE_EXECUTION_CONTEXT_H
