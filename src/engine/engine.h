//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

#include "engine_init_arguments.h"
#include "engine_execution_arguments.h"

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
struct scene;


struct engine* engine_create(struct engine_init_arguments arguments);
void engine_destroy(struct engine *this);

void engine_execute(struct engine *this, struct engine_execution_arguments arguments);

struct engine_init_context* engine_get_init_context(const struct engine *this);
struct engine_execution_context* engine_get_execution_context(const struct engine *this);
struct subsystem_collection* engine_get_subsystems(const struct engine* this);

#endif //MYCOOLGAMEENGINE_ENGINE_H
