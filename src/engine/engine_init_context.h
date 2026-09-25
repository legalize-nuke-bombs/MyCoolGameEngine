//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_INIT_CONTEXT_H
#define MYCOOLGAMEENGINE_ENGINE_INIT_CONTEXT_H

#include "engine_init_arguments.h"

struct engine_init_context;
struct engine;

struct engine_init_context* engine_init_context_try_create(struct engine *engine, struct engine_init_arguments arguments);
void engine_init_context_destroy(struct engine_init_context *this);

void engine_init_context_awake(struct engine_init_context *this);
void engine_init_context_disable(struct engine_init_context *this);

struct SDL_Renderer* engine_init_context_get_native_renderer(const struct engine_init_context *this);
struct renderer_pipeline* engine_init_context_get_renderer_pipeline(const struct engine_init_context *this);
struct interpreter* engine_init_context_get_interpreter(const struct engine_init_context *this);
struct component_fabric* engine_init_context_get_component_fabric(const struct engine_init_context *this);
struct devices* engine_init_context_get_devices(struct engine_init_context *this);

#endif //MYCOOLGAMEENGINE_ENGINE_INIT_CONTEXT_H
